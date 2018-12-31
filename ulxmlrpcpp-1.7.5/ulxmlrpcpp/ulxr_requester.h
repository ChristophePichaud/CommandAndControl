/***************************************************************************
             ulxr_requester.h  -  send rpc request ("rpc-client")
                             -------------------
    begin                : Sat Mar 23 2002
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_requester.h 942 2006-12-31 13:10:54Z ewald-arnold $

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

#ifndef ULXR_REQUESTER_H
#define ULXR_REQUESTER_H

#include <exception>

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header
#include <ulxmlrpcpp/ulxr_call.h>
#include <ulxmlrpcpp/ulxr_mutex.h>
#include <ulxmlrpcpp/ulxr_response.h>


namespace ulxr {


class Protocol;
class Connection;
class Exception;

namespace hidden {


/** Internal helper class, not intended for public use.
 * @internal
 */
class ULXR_API_DECL0 ReceiverWrapperBase
{
  public:

    virtual ~ReceiverWrapperBase();

    virtual void receive(const MethodResponse &resp) const = 0;
};


/** Internal helper class template, not intended for public use.
 * @internal
 */
template <class T>
class ReceiverWrapper : public ReceiverWrapperBase
{
  public:

   typedef void (T::*PMF)(const MethodResponse &resp);

   virtual ~ReceiverWrapper()
   {}

   virtual void receive(const MethodResponse &resp) const
   {
      (obj->*adr) (resp);
   }

  public:

   ReceiverWrapper(T *o, PMF a)
    : obj(o), adr(a)
   {}

  private:
   T    *obj;
   PMF   adr ;

 private:
    // forbid them all due to internal pointers
    const ReceiverWrapper& operator= (const ReceiverWrapper&);
    ReceiverWrapper (const ReceiverWrapper&);
};


typedef void (*StaticReceiver_t)(const MethodResponse &);

typedef ReceiverWrapperBase *DynamicReceiver_t;    // call Wrappers receiver

/** Wrapper class for the receiver types.
  */
class ULXR_API_DECL0 Receiver_t
{
 public:
  /** Receiver is a function or static method.
    * @param recv function adress
    */
    Receiver_t(StaticReceiver_t recv);

  /** Receiver is a class method.
    * @param recv method wrapper
    */
    Receiver_t(DynamicReceiver_t recv);

  /** Call the actual receiver function.
    * @param resp method response which is sent to the receiver
    */
    void receive(const MethodResponse &resp);

  /** Free memory.
    */
    void free();

 protected:

   StaticReceiver_t    static_recv;
   DynamicReceiver_t   dynamic_recv;
/*
 private:
    // forbid them all due to internal pointers
    const Receiver_t& operator= (const Receiver_t&);
    Receiver_t (const Receiver_t&);
*/
};


}  // namespace hidden


struct DispatcherData;


/** XML RPC Requester (rpc client).
  * The requester takes the MethodCall, converts it to xml and sends
  * it over the connection. It then waits for the response which must
  * be further processes by the caller.
  * @ingroup grp_ulxr_rpc
  */
class ULXR_API_DECL0 Requester
{
 public:

 /** Constructs a requester.
   * @param  prot        pointer to an existing Connection
   * @param  wbxml_mode  true: data is sent as wbxml
   */
   Requester(Protocol* prot, bool wbxml_mode = false);

 /** Destroys the requester.
   */
   virtual ~Requester();

 /** Performs a virtual call to the remote method
   * "behind" the connection.
   * @param   call      the data for the call
   * @param   resource  resource for rpc on remote host
   * @return the methods response
   */
   MethodResponse call (const MethodCall& call,
                        const CppString &resource);

 /** Performs a virtual call to the remote method
   * "behind" the connection.
   * @param   call      the data for the call
   * @param   resource  resource for rpc on remote host
   * @param   user      user name
   * @param   pass      password
   * @return the methods response
   */
   MethodResponse call (const MethodCall& call,
                        const CppString &resource,
                        const CppString &user,
                        const CppString &pass);

#ifdef ULXR_MULTITHREADED

/** Performs a virtual call to the remote method
   * "behind" the connection and passes handling of result
   * to a worker function in multitasking mode.
   * This method itself returns immediately.
   * @attention the dispatcher object itself is not thread-safe!
   * @param   call      the data for the call
   * @param   resource  resource for rpc on remote host
   * @param   recv      receiver function
   */
   void call (const MethodCall& call,
              const CppString &resource,
              hidden::Receiver_t recv);

 /** Performs a virtual call to the remote method
   * "behind" the connection and passes handling of result
   * to a worker function in multitasking mode.
   * This method itself returns immediately.
   * @attention the dispatcher object itself is not thread-safe!
   * @param   call      the data for the call
   * @param   resource  resource for rpc on remote host
   * @param   user      user name
   * @param   pass      password
   * @param   recv      receiver function
   */
   void call (const MethodCall& call,
              const CppString &resource,
              const CppString &user,
              const CppString &pass,
              hidden::Receiver_t recv);

#endif // ULXR_MULTITHREADED

 /** Performs a virtual call to the remote method
   * without waiting for (or accepting) a reply.
   * @param   call      the data for the call
   * @param   resource  resource for rpc on remote host
   */
   void transmit (const MethodCall& call,
                  const CppString &resource);

 /** Performs a virtual call to the remote method
   * without waiting for (or accepting) a reply.
   * @param   call      the data for the call
   * @param   resource  resource for rpc on remote host
   * @param   user      user name
   * @param   pass      password
   */
   void transmit (const MethodCall& call,
                  const CppString &resource,
                  const CppString &user,
                  const CppString &pass);

 /** Waits for the response from the remote server.
   * @param  conn   connection to wait for data
   * @param  wbxml  true: use wbxml mode
   * @return methode response
   */
   static MethodResponse waitForResponse(Protocol *conn, bool wbxml);

#ifdef ULXR_MULTITHREADED

 /** Returns the number of pending requests.
   * @return number of still pending requests in threads
   */
   unsigned numPendingRequests() const;

#endif

 /** Returns true is wbxml is to be used
   * @return tre when in wbxmlmode
   */
   bool isWbXml() const;

 protected:

 /** Sends the call data to the remote method.
   * @param   call      the data for the call
   * @param   resource  resource for rpc on remote host
   */
   void send_call(const MethodCall& call,
                  const CppString &resource);

 /** Waits for the response from the remote server.
   * @return methode response
   */
   MethodResponse waitForResponse();

#ifdef ULXR_MULTITHREADED

/** Waits for the response and dispatches it in multitasked mode.
   * @attention the dispatcher object itself is not thread-safe
   * but your reciever function has to!
   * @param   call      the data for the call
   * @param   resource  resource for rpc on remote host
   * @param   recv      receiver method
   */
   void startDispatch(const MethodCall& call,
                      const CppString &resource,
                      hidden::Receiver_t recv);

#endif // ULXR_MULTITHREADED

 private:

#ifdef ULXR_MULTITHREADED

   friend void *dispatchThreaded(DispatcherData *data);

 /** Forwards an unknown exception from  \c dispatchThreaded().
   * @note This function @b must return to free resources and return an
   * error value afterwards.
   */
   virtual void forwardException();

 /** Forwards an ulxr::Exception from  \c dispatchThreaded().
   * @note This function @b must return to free resources and return an
   * error value afterwards.
   * @param ex  Exception
   */
   virtual void forwardException(const Exception &ex);

 /** Forwards a std::exception from  \c dispatchThreaded().
   * @note This function @b must return to free resources and return an
   * error value afterwards.
   * @param ex  Exception
   */
   virtual void forwardException(const std::exception &ex);

 /** Increments the number of pending requests.
   */
   void incPending();

 /** Decrements the number of pending requests.
   */
   void decPending();

#endif

   bool               wbxml_mode;
   Protocol          *protocol;
   unsigned           cntPendingRequests;
   Mutex              pendingMutex;
};


/** Creates a wrapper object to a method of a worker class.
  * @param w     reference to worker class
  * @param pmf   adress of member function in worker class
  * @return pointer to wrapper object
  */

#ifndef _MSC_VER

template <class T>
inline hidden::ReceiverWrapperBase*
make_receiver(T &w, typename hidden::ReceiverWrapper<T>::PMF pmf)
{
  return new hidden::ReceiverWrapper<T> (&w, pmf);
}

#else // work around m$vc bug

template <class T, class U>
inline hidden::ReceiverWrapper<T>*
make_receiver(T &w, U pmf)
{
  return new hidden::ReceiverWrapper<T> (&w, pmf);
}

#endif

/** Specialisation of template to achive uniform look in receiver creation.
  * @param ptr   pointer to receiver function
  * @return pointer to the function
  */
inline hidden::StaticReceiver_t
make_receiver(hidden::StaticReceiver_t ptr)
{
  return ptr;
}


}  // namespace ulxr


#endif // ULXR_REQUESTER_H
