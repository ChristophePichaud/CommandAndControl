/***************************************************************************
                    ulxr_http_server.h  -  a simple http server
                             -------------------
    begin                : Sam Apr 20 2002
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_http_server.h 1083 2007-09-29 11:14:11Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/

#ifndef ULXR_HTTP_SERVER_H
#define ULXR_HTTP_SERVER_H

#ifndef ULXR_OMIT_TCP_STUFF

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <vector>
#include <map>
#include <cstdio>


namespace ulxr {


class HttpProtocol;
class Dispatcher;
class Exception;
class MethodHandler;
class CachedResource;

/** A simple server for an http connection.
  * This implementation only honors the basic http methods PUT, GET, POST and DELETE.
  * It is not intended as a replacement for a full featured web server.
  * @ingroup grp_ulxr_http
  */
class ULXR_API_DECL0 HttpServer
{
 public:

   class ThreadData;

 /** Constructs a HttpServer in single threaded mode.
   * @param  prot    pointer to an existing http protocol
   * @param  wbxml   true: use binary xml for xml-rpc
   */
   HttpServer (HttpProtocol* prot, bool wbxml = false);

#ifdef ULXR_MULTITHREADED

 /** Constructs a HttpServer in multithreaded mode.
   * @param  prot   pointer to an existing http protocol
   * @param  num    number of parallel threads
   * @param  wbxml  true: use binary xml for xml-rpc
   */
   HttpServer (HttpProtocol* prot, unsigned num, bool wbxml = false);

#endif

 /** Destroys the http server.
   */
   virtual ~HttpServer ();

 /** Runs a very simple http server.
   * You should think of it as a file server that is
   * capable of reading/writing/deleting/posting files, not much more.
   * But it also includes the routing of RPC request!
   * Derived classes might of course implement for example running
   * scripts or do something else more advanced.
   * @return in multithreaded mode: number of created threads. Otherwise after termination: 0.
   */
   std::size_t runPicoHttpd();

 /** Sets the root of the pico http server.
   * @param   root   top level directory of the server
   */
   void setHttpRoot(const CppString &root);

 /** Adds another cross reference from a http resource to a realm.
   * The default implementation assumes that all added resources are paths so the last character
   * should be a slash.
   * It then tries to find these paths at the beginning of paths in queries assuming that
   * all files below the according path belong to the same realm.
   * @param  path   name of resource
   * @param  realm  name of realm
   */
   virtual void addRealm(const CppString &path, const CppString &realm);

 /** Gets realm name of a resource.
   * The default implementation is very simple and only does a string compare with the given path
   * and the realm-to-resource mapping. The longest match is taken.
   * @param  path   name of resource which must match exactly.
   * @return name of realm or "" if unavailable.
   */
   virtual CppString getRealm(const CppString &path) const;

 /** Sets a new handler for a http method.
   * The handler will be invoked after the header has been read completely.
   * All information like http fields can be queried from the connection.
   * If a handler was willing (not necessarily able) to perform the method
   * it must return "true". Otherwise the next handler will be invoked.
   *
   * \attention The handler must completely process the transaction: reading the rest
   * of the message body and send the response.
   *
   * @param  name     method name. Only GET, PUT, POST, DELETE are supported
   * @param  handler  pointer to handler object. Ownership is taken.
   */
   void addHttpHandler(const CppString &name,
                       MethodHandler *handler);

 /** Removes a handler for a http method.
   * @param  name     method name. Only GET, PUT, POST, DELETE are supported
   * @param  handler  handler function.
   */
   void removeHttpHandler(const CppString &name,
                          MethodHandler *handler);

 /** Adds a resource
   * @param  cache pointer to the resource
   */
   void addResource(CachedResource *cache);

 /** Gets a resource .
   * @param  name  name of the resource
   */
   CachedResource * getResource(const CppString& name);

 /** Sets a dispatcher for incoming RPC calls.
   * @param  disp   pointer to RPC dispatcher
   */
   void setRpcDispatcher(Dispatcher *disp);

 /** Requests the server loop to terminate.
   * Note: only a flag is set which is checked before waiting for the next call.
   * For a remote shutdown implement a rpc method with calls this method
   * or from outside call \c terminateAllThreads().
   */
   void requestTermination();

 /** Adds another potential username, password and "range" for this connection (server mode).
   * @param  user   Username
   * @param  pass   Password
   * @param  realm  Synonym for area which shall be accessed
   */
   void addAuthentication(const CppString &user,
                          const CppString &pass,
                          const CppString &realm);

#ifdef ULXR_MULTITHREADED

  /** Gets the number of installed threads.
    * @return number of threads
    */
    unsigned numThreads() const;

  /** Terminates all threads.
    * The threads only get a message and must terminate themselves.
    * @param time  planned: time in ms to wait before killing.
    */
    void terminateAllThreads(unsigned time = 0);

  /** Signals termination and shuts down all ports.
    * This is a rather rude way to terminate to stop the servers
    * @param time  planned: time in ms to wait before killing.
    */
    void shutdownAllThreads(unsigned time = 0);

  /** Waits for all threads to complete.
    * @param  term  true: signal all threads to shut down as soon as possible.
    * @param  stat  true: print some statistical data to stdout.
    */
    void waitAsync(bool term, bool stat = false);

#endif // ULXR_MULTITHREADED

 protected:

#ifdef ULXR_MULTITHREADED

  /** Prints some statistics about the running threads.
    */
    void printStatistics() const;

#endif // ULXR_MULTITHREADED

  /** Forwards error that happen within threads.
    * @param ex   Exception that occured
    */
    virtual void forwardThreadedError(const Exception &ex) const;

 /** Executes the appropriate method when the connection acts as http server.
   * @param  prot       pointer to the connecthion object
   * @param  conn_pending_data already received chunk from connection
   * @param  name      the method (GET, PUT ..)
   * @param  resource  name of the resource
   */
   virtual void executeHttpMethod(HttpProtocol *prot,
                                  Cpp8BitString &conn_pending_data,
                                  const CppString& name,
                                  const CppString& resource);

 /** Executes a http method not known to ulxmlrpcpp.
   * The default is to throw an according exception.
   * @param  prot       pointer to the connecthion object
   * @param  conn_pending_data already received chunk from connection
   * @param  name      the method (GET, PUT ..)
   * @param  resource  name of the resource
   */
   virtual void executeUnknownHttpMethod(HttpProtocol *prot,
                                         Cpp8BitString &conn_pending_data,
                                         const CppString& name,
                                         const CppString& resource);

 /** Executes the GET method when the connection acts as http server.
   * @param  prot       pointer to the connecthion object
   * @param  resource  name of the resource
   */
   virtual void executeHttpGET(HttpProtocol *prot,
                               const CppString& resource);

 /** Executes the PUT method when the connection acts as http server.
   * @param  prot       pointer to the connecthion object
   * @param  conn_pending_data already received chunk from connection
   * @param  resource  name of the resource
   */
   virtual void executeHttpPUT(HttpProtocol *prot,
                               Cpp8BitString &conn_pending_data,
                               const CppString& resource);

 /** Executes the POST method when the connection acts as http server.
   * If the request resource is the same as "rpc_resource_root" and a dispatcher
   * is available then executeHttpRPC is invoked.
   * @param  prot       pointer to the connecthion object
   * @param  conn_pending_data already received chunk from connection
   * @param  resource  name of the resource
   */
   virtual void executeHttpPOST(HttpProtocol *prot,
                                Cpp8BitString &conn_pending_data,
                                const CppString& resource);

 /** Executes the RPC server.
   * @param  prot               pointer to the connecthion object
   * @param  conn_pending_data  already received chunk from connection
   */
   virtual void executeHttpRPC(HttpProtocol *prot,
                               Cpp8BitString &conn_pending_data);

 /** Executes the DELETE method when the connection acts as http server.
   * @param  prot      pointer to the connection object
   * @param  resource  name of the resource
   */
   virtual void executeHttpDELETE(HttpProtocol *prot,
                                  const CppString& resource);

 /** Performs a complete http transaction.
   * A request is read from the connection, processes and the result
   * sent back to the client.
   * @param  prot pointer to the connecthion object
   */
   void performHttpTransaction(HttpProtocol *prot);

 /** Processes all the http headers after the header has been completely
   * read.
   * @param  prot            pointer to the connecthion object
   * @param  head_resource   resource in http header
   * @param  head_method     method name name in http header
   * @param  head_version    version in http header
   */
   virtual void interpreteHttpHeader(HttpProtocol *prot,
                                     CppString &head_resource,
                                     CppString &head_method,
                                     CppString &head_version);

 /** Strips protocol and host name from the resource
   * @param  resource  name of the resource
   * @return actual resource
   */
   CppString stripResource(const CppString &resource);

 /** Checks if the resource contains a valid path.
   * ".." may not point "behind" the http root.
   * @param  resource  name of the resource
   */
   virtual void checkValidPath(const CppString &resource);

 /** creates the local filename from the httpd root and the resource name.
   * @param  resource  name of the local resource
   * @return name of the according local file
   */
   CppString createLocalName(const CppString &resource);

 /** Tries to guess a mime type from the given file.
   * @param  name   filename
   * @return mime type
   */
   virtual CppString guessMimeType(const CppString &name) const;

  private:

 /** Called when the thread is started and enters the server loop
  */
   virtual void enterLoop();

 /** Called when the server loop is terminating
  */
   virtual void leaveLoop();

 /** Perform some user defined action before processing the next request.
   */
   virtual void beforeHttpTransaction();

 /** Perform some user defined action after processing the request.
   */
   virtual void afterHttpTransaction();

 /** Runs a very simple http server.
   * @param  prot  pointer to the private connection
   * @param  td    pointer to private thread data
   * @return in multithreaded: number of created threads. Otherwise after termination: 0.
   */
   std::size_t runPicoHttpd(HttpProtocol *prot, ThreadData *td = 0);

#ifdef ULXR_MULTITHREADED

 /** Helper method to start a thread.
   * @param td  pointer to thread data
   */
   static void *startThread(ThreadData *td);

 /** Deletes all allocated thread objects.
   */
   void releaseThreads();

 /** Lauch the http server threads which waits for requests.
   * @return  number of threads started
   */
   unsigned dispatchAsync();

#endif // ULXR_MULTITHREADED

 /** Deletes all registered http method handlers.
   */
   void releaseHandlers(std::vector<MethodHandler*> &handlers);

 /** Sets up internal variables.
   */
   void init();

 private:

   CppString    rpc_resource_root;
   CppString    http_root_dir;
   bool         pico_shall_run;
   bool         wbxml_mode;

   Dispatcher                  *rpc_dispatcher;
   HttpProtocol                *base_protocol;
#ifdef ULXR_MULTITHREADED
   std::vector<ThreadData*>     threads;
#endif

   std::vector<MethodHandler*>  getHandlers;
   std::vector<MethodHandler*>  putHandlers;
   std::vector<MethodHandler*>  postHandlers;
   std::vector<MethodHandler*>  deleteHandlers;

   std::vector<CachedResource*>  resources;

   std::map<CppString, CppString>  realmXrefs;

 private:

   // forbid them all due to internal pointers
   HttpServer(const HttpServer&);
   HttpServer& operator=(const HttpServer&);
};


namespace hidden {


/** Internal helper class, not intended for public use.
 */
class ULXR_API_DECL0 MethodHandlerWrapperBase
{
  public:


  /** Destroys wrapper
    */
    virtual ~MethodHandlerWrapperBase()
    {}

  /** Forwards the call to the according handler method.
    * @param  prot       pointer to the according connection
    * @param  method     the method in use
    * @param  resource   name of the resource
    * @param  conn_data  already received chunk of the message body.
    *                    Needed for the methods PUT or POST
    * @return  true: request was handled. false: try next handler.
    */
    virtual bool handle(HttpProtocol *prot,
                        const CppString &method,
                        const CppString &resource,
                        const Cpp8BitString &conn_data) const = 0;
};


/** Internal helper class template, not intended for public use.
 */
template <class T>
class MethodHandlerWrapper : public MethodHandlerWrapperBase
{
  public:

 /** Pointer to member function to handle a http request.
   * @param  prot       pointer to the according server
   * @param  method     the method in use
   * @param  resource   name of the resource
   * @param  conn_data  already received chunk of the message body.
   *                    Needed for the methods PUT or POST
   * @return  true: request was handled. false: try next handler.
   */
   typedef bool (T::*PMF)(HttpProtocol *prot,
                          const CppString &method,
                          const CppString &resource,
                          const Cpp8BitString &conn_data);

 /** Destroy method handler wrapper
   */
   virtual ~MethodHandlerWrapper()
   {
   }

 /** Handle a chunk of received data.
   * @param  prot       pointer to the according server
   * @param  method     the method in use
   * @param  resource   name of the resource
   * @param  conn_data  already received chunk of the message body.
   *                    Needed for the methods PUT or POST
   * @return  true: request was handled. false: try next handler.
   */
   virtual bool handle(HttpProtocol *prot,
                       const CppString &method,
                       const CppString &resource,
                       const Cpp8BitString &conn_data) const
   {
      return (obj->*adr) (prot, method, resource, conn_data);
   }

  public:

 /** Construct a handler wrapper.
   * @param  o    handling object
   * @param  a    desired method in this object
   */
   MethodHandlerWrapper(T *o, PMF a)
     : obj(o), adr(a)
   {
   }

  private:
   T    *obj;
   PMF   adr ;
};

/**
  * @param  prot       pointer to the according server
  * @param  method     the method in use
  * @param  resource   name of the resource
  * @param  conn_data  already received chunk of the message body.
  *                    Needed for the methods PUT or POST
  * @return  true: request was handled. false: try next handler.
  */
typedef bool (*StaticMethodHandler)(HttpProtocol *prot,
                                    const CppString &method,
                                    const CppString &resource,
                                    const Cpp8BitString &conn_data);

typedef MethodHandlerWrapperBase *DynamicMethodHandler;    // call Wrappers handler


} // namspace hidden


/** Wrapper class for the method handlers.
  */
class ULXR_API_DECL0 MethodHandler
{
 public:
  /** Handler is a function or static method.
    * @param handler function adress
    */
    MethodHandler(hidden::StaticMethodHandler handler);

  /** Handler is a class method.
    * @param handler  method wrapper
    */
    MethodHandler(hidden::DynamicMethodHandler handler);

  /** Destroys the handler.
    */
    virtual ~MethodHandler();

  /** Call the actual handler function.
    * @param  prot       pointer to the according server
    * @param  method     the method in use
    * @param  resource   name of the resource
    * @param  conn_data  already received chunk of the message body.
    *                    Needed for the methods PUT or POST
    * @return  true: request was handled. false: try next handler.
    */
    bool handle(HttpProtocol *prot,
                const CppString &method,
                const CppString &resource,
                const Cpp8BitString &conn_data);

  private:

    hidden::StaticMethodHandler    static_handler;
    hidden::DynamicMethodHandler   dynamic_handler;

    // forbid them all due to internal pointers
    const MethodHandler& operator= (const MethodHandler&);
    MethodHandler (const MethodHandler&);
};


#ifndef _MSC_VER

/** Specialisation of template to achive uniform look in handler creation.
  * @param w     reference to handler object
  * @param pmf   pointer to member function
  * @return handler object
  */
template <class T>
inline MethodHandler*
make_methodhandler(T &w, typename hidden::MethodHandlerWrapper<T>::PMF pmf)
{
  return new MethodHandler(new hidden::MethodHandlerWrapper<T> (&w, pmf));
}

#else // work around m$vc bug

template <class T, class U>
inline MethodHandler*
make_methodhandler(T &w, U pmf)
{
  return new MethodHandler(new hidden::MethodHandlerWrapper<T> (&w, pmf));
}

#endif

/** Specialisation of template to achive uniform look in handler creation.
  * @param ptr   pointer to handler function
  * @return handler object
  */
inline ULXR_API_IMPL(MethodHandler*)
make_methodhandler(hidden::StaticMethodHandler ptr)
{
  return new MethodHandler(ptr);
}


}  // namespace ulxr


#endif // ULXR_OMIT_TCP_STUFF


#endif // ULXR_HTTP_SERVER_H
