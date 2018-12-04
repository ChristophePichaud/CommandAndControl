/***************************************************************************
           ulxr_mtrpc_server.h  -  a simple multithreaded rpc server
                             -------------------
    begin                : Wed Oct 10 2003
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_mtrpc_server.h 1076 2007-09-02 08:07:32Z ewald-arnold $

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

#ifndef ULXR_MTRPC_SERVER_H
#define ULXR_MTRPC_SERVER_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#ifdef ULXR_MULTITHREADED

#include <vector>

#include <ulxmlrpcpp/ulxr_dispatcher.h>
#include <ulxmlrpcpp/ulxr_method_adder.h>

#ifdef __WIN32__
#include <process.h>
#include <windows.h>
#endif

#ifdef __unix__
#include <pthread.h>
#endif


namespace ulxr {


class Exception;
class Protocol;

/** A multi threaded xml rpc server.
  * @ingroup grp_ulxr_rpc
  */
class ULXR_API_DECL0 MultiThreadRpcServer : public MethodAdder
{
  public:
    class ThreadData;

    typedef void* (*ThreadLoopFunction)(ThreadData*);

  /** Constructs the rpc server.
    * @param  prot        communication object
    * @param  num         number of simultanous threads to handle requests
    * @param   wbxml_mode  true: data is sent as wbxml
    */
    MultiThreadRpcServer(Protocol *prot, unsigned num, bool wbxml_mode = false);

  /** Destrcuts the rpc server.
    */
    virtual ~MultiThreadRpcServer();

  /** Lauch the rpc server threads which wait for requests.
    * @return  number of threads started
    */
    unsigned dispatchAsync();

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

 /** Adds a user defined (static) method to the dispatcher.
   * You access a remote method by sending the "official" name. Sometimes
   * a method accepts different parameter sets (overloading in C++).
   * In this case you add the according signature. Finally you can
   * add a description to show the usage of this method.
   * @param  adr            the pointer to the implementation of the method
   * @param  ret_signature  the signature of the return value
   * @param  name           the name of the method
   * @param  signature      the signature of the parameters
   * @param  help           short usage description
   */
   void addMethod (MethodAdder::StaticMethodCall_t adr,
                   const CppString &ret_signature,
                   const CppString &name,
                   const CppString &signature,
                   const CppString &help);

 /** Adds a user defined (dynamic) method to the dispatcher.
   * You access a remote method by sending the "official" name. Sometimes
   * a method accepts different parameter sets (overloading in C++).
   * In this case you add the according signature. Finally you can
   * add a description to show the usage of this method.
   * @param  wrapper        the pointer to the wrapper to the method.
   *                        Important: Dispatcher owns now and deletes this wrapper object!
   * @param  ret_signature  the signature of the return value
   * @param  name           the name of the method
   * @param  signature      the signature of the parameters
   * @param  help           short usage description
   */
   void addMethod (MethodAdder::DynamicMethodCall_t wrapper,
                   const CppString &ret_signature,
                   const CppString &name,
                   const CppString &signature,
                   const CppString &help);

 /** Adds a system internal method to the dispatcher.
   * You access a remote method by sending the "official" name. Sometimes
   * a method accepts different parameter sets (overloading in C++).
   * In this case you add the according signature. Finally you can
   * add a description to show the usage of this method.
   * @param  adr            the pointer to the implementation of the method
   * @param  ret_signature  the signature of the return value
   * @param  name           the name of the method
   * @param  signature      the signature of the parameters
   * @param  help           short usage description
   */
   void addMethod (MethodAdder::SystemMethodCall_t adr,
                   const CppString &ret_signature,
                   const CppString &name,
                   const CppString &signature,
                   const CppString &help);

 /** Adds a user defined (static) method to the dispatcher.
   * You access a remote method by sending the "official" name. Sometimes
   * a method accepts different parameter sets (overloading in C++).
   * In this case you add the according signature. Finally you can
   * add a description to show the usage of this method.
   * @param  adr            the pointer to the implementation of the method
   * @param  ret_signature  the signature of the return value
   * @param  name           the name of the method
   * @param  signature      the signature of the parameters
   * @param  help           short usage description
   */
   void addMethod (MethodAdder::StaticMethodCall_t adr,
                   const Signature &ret_signature,
                   const CppString &name,
                   const Signature &signature,
                   const CppString &help);

 /** Adds a user defined (dynamic) method to the dispatcher.
   * You access a remote method by sending the "official" name. Sometimes
   * a method accepts different parameter sets (overloading in C++).
   * In this case you add the according signature. Finally you can
   * add a description to show the usage of this method.
   * @param  wrapper        the pointer to the wrapper to the method.
   *                        Important: Dispatcher owns now and deletes this wrapper object!
   * @param  ret_signature  the signature of the return value
   * @param  name           the name of the method
   * @param  signature      the signature of the parameters
   * @param  help           short usage description
   */
   void addMethod (MethodAdder::DynamicMethodCall_t wrapper,
                   const Signature &ret_signature,
                   const CppString &name,
                   const Signature &signature,
                   const CppString &help);

 /** Adds a system internal method to the dispatcher.
   * You access a remote method by sending the "official" name. Sometimes
   * a method accepts different parameter sets (overloading in C++).
   * In this case you add the according signature. Finally you can
   * add a description to show the usage of this method.
   * @param  adr            the pointer to the implementation of the method
   * @param  ret_signature  the signature of the return value
   * @param  name           the name of the method
   * @param  signature      the signature of the parameters
   * @param  help           short usage description
   */
   void addMethod (MethodAdder::SystemMethodCall_t adr,
                   const Signature &ret_signature,
                   const CppString &name,
                   const Signature &signature,
                   const CppString &help);

 /** Removes a method if available
   * @param name   method name
   */
   void removeMethod(const CppString &name);

  protected:

 /** Forwards error that happen within threads.
   * @param ex   Exception that occured
   */
   virtual void forwardThreadedError(const Exception &ex) const;

  /* The threaded function that does the work.
   * @param  conn  pointer to communication object
   * @param  td    pointer to working data
   */
   void *serverLoop(Protocol *protocol, ThreadData *td);

 /** Prints some statistics about the running threads.
   */
   void printStatistics() const;

 /** Called when the thread is started and enters the server loop
  */
   virtual void enterLoop();

 /** Called when the server loop is terminating
  */
   virtual void leaveLoop();

 /** Processes a call after it has been recieved and before it is dispatched.
   * @param  call   last received call
   */
   virtual void preProcessCall(MethodCall &call);

 /** Processes a method response before it is sent back.
   * @param  resp   response to send back
   */
   virtual void preProcessResponse(MethodResponse &resp);

  private:

 /** Helper method to start a thread.
   * @param td  pointer to thread data
   */
   static void *startThread(ThreadData *td);

 /** Deletes all allocated thread objects.
   */
   void releaseThreads();

   typedef void *(Thread_t) (ThreadData*);

   std::vector<ThreadData*>       threads;
   Dispatcher                     dispatcher;
   bool                           wbxml_mode;

 private:

   // forbid them all due to internal pointers
   MultiThreadRpcServer(const MultiThreadRpcServer&);
   MultiThreadRpcServer& operator=(const MultiThreadRpcServer&);
};


}  // namespace ulxr


#endif // ULXR_MULTITHREADED


#endif // ULXR_MTRPC_SERVER_H
