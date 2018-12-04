/***************************************************************************
         ulxr_dispatcher.h  -  answer rpc requests ("rpc-server")
                             -------------------
    begin                : Sat Mar 23 2002
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_dispatcher.h 1026 2007-07-25 07:48:09Z ewald-arnold $

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

#ifndef ULXR_DISPATCHER_H
#define ULXR_DISPATCHER_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <ulxmlrpcpp/ulxr_call.h>
#include <ulxmlrpcpp/ulxr_response.h>
#include <ulxmlrpcpp/ulxr_method_adder.h>


namespace ulxr {


class Protocol;
class Connection;
class Struct;
class Signature;


/** XML RPC Dispatcher (rpc server).
  * The dispacher waits for incoming requests. The request is parsed
  * and the according method is called afterwards the response is
  * converted into xml and sent back to the requester.
  * Direct use of this class is intended for proprietary connections.
  * If you go the usual way via http you ought to use the HttpServer class
  * and run "picoHttpd"
  * @ingroup grp_ulxr_rpc
  */
class ULXR_API_DECL0 Dispatcher : public MethodAdder
{
 public:

   struct ULXR_API_DECL0 MethodCallDescriptor
   {

     friend class Dispatcher;

   /** Constructs a method call descriptor.
     * @param call  method call data
     */
     MethodCallDescriptor(const MethodCall &call);

   /** Constructs a method call descriptor.
     * @param calltype   type of descriptor
     * @param ret_sig    signature of return type
     * @param name       method name
     * @param signature  signature of parameters
     * @param help       description of method
     */
     MethodCallDescriptor(CallType calltype,
                          const CppString &ret_sig,
                          const CppString &name,
                          const CppString &signature,
                          const CppString &help);

   /** Compares two method call descriptors.
     * @return true: both are NOT equal
     */
     bool operator == (const MethodCallDescriptor &desc) const
       { return getSignature(true, false) == desc.getSignature(true, false); }

   /** Compares two method call descriptors.
     * @return true: both are equal
     */
     bool operator < (const MethodCallDescriptor &desc) const
       { return getSignature(true, false) < desc.getSignature(true, false); }

   /** Gets the signature of the method call.
     * @param with_name    includes also the name of the method (in the middle)
     * @param with_return  includes also the return type (at the begining)
     * @return the signature as string
     */
     CppString getSignature(bool with_name, bool with_return) const;

   /** The the invocation counter.
     * @return how many this methods was invoked
     */
     unsigned long getInvoked() const;

   /** Increments the invocation counter by one
     */
     void incInvoked() const;

   /** Returns the status of availability
     * @return true: method is available
     */
     bool isEnabled() const;

   /** Sets the status of availability
     * @param ena  true: method is available
     */
     void setEnabled(bool ena) const;

   /** Gets the method name
     * @return method name
     */
     CppString getMethodName() const;

   /** Gets the parameter signature
     * @return parameter signature
     */
     CppString getParameterSignature() const;

   /** Gets the return value signature
     * @return return value signature
     */
     CppString getReturnValueSignature() const;

   /** Gets the documentation
     * @return documentation
     */
     CppString getDocumentation() const;

   /** Gets the call type
     * @return call type
     */
     CallType getCallType() const;

    private:

     CallType         calltype;
     CppString        method_name;
     CppString        signature;
     CppString        return_signature;
     CppString        documentation;

     mutable unsigned long  invoked;
     mutable bool           enabled;
   };

 protected:

   typedef std::map<MethodCallDescriptor, MethodCall_t> MethodCallMap;

   friend void free_dynamic_method(const MethodCallMap::value_type &);

   friend struct MethodCallDescriptor;

 public:

 /** Constructs a dispatcher.
   * @param  prot        pointer to an existing Protocol
   * @param  wbxml_mode  true: data is sent as wbxml
   */
   Dispatcher (Protocol* prot = 0, bool wbxml_mode = false);

 /** Destroys the dispatcher.
   * Important: Also frees wrapper objects to worker classes since Dispatcher
   * owns them!
   */
   virtual ~Dispatcher ();

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
   void addMethod (StaticMethodCall_t adr,
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
   void addMethod (DynamicMethodCall_t wrapper,
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
   void addMethod (SystemMethodCall_t adr,
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
   void addMethod (StaticMethodCall_t adr,
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
   void addMethod (DynamicMethodCall_t wrapper,
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
   void addMethod (SystemMethodCall_t adr,
                   const Signature &ret_signature,
                   const CppString &name,
                   const Signature &signature,
                   const CppString &help);

 /** Waits for an incoming method call.
   * @param timeout the timeout value [sec] (0 - no timeout)
   * @return the complete call data
   */
   virtual MethodCall waitForCall(int timeout = 0);

 /** Dispatches the call to the according implementation
   * @param  call  the call data
   * @return the complete response data
   */
   virtual MethodResponse dispatchCall(const MethodCall &call) const;

 /** Dispatches the call to the according implementation
   * <p> Does not catch any exceptions
   * @see dispatchCall
   * @param  call  the call data
   * @return the complete response data
   */
   virtual MethodResponse dispatchCallLoc(const MethodCall &call) const;

 /** Sends the response back to the requester.
   * @param  resp        the response data
   */
   virtual void sendResponse(const MethodResponse &resp);

 /** Returns the current connection.
   * @return pointer to the current connection.
   */
   Protocol* getProtocol() const;

 /** Sets the current connection.
   * @param  prot  pointer to the new protcol
   */
   void setProtocol(Protocol *prot);

 /** Gets the number of available methods.
   * @return number of methods
   */
   unsigned numMethods() const;

 /** Gets method data
   * @param index  index for method
   * @return method data
   */
   const MethodCallDescriptor * const getMethod(unsigned index);

 /** Checks if a method is available
   * @param call  call data
   * @return true if method is available
   */
   bool hasMethod(const MethodCall &call) const;

 /** Removes a method if available
   * @param name   method name
   */
   void removeMethod(const CppString &name);

 protected:

 /** Adds a methode descriptor to the dispatcher.
   * @param  desc   descriptor data
   * @param  mct    pointer to the function
   */
   void addMethodDescriptor (const MethodCallDescriptor &desc,
                             MethodCall_t mct);

 /** Adds internal "system.x" methods.
   */
   void setupSystemMethods();

 /** Enable pretty-printed xml responses
   * @param  calldata  parameters, must be one bool with new status
   * @param  disp      pointer to actual dispatcher
   * @return void
   */
   static MethodResponse xml_pretty_print(const MethodCall &calldata,
                                          const Dispatcher *disp);

 /** Lists all methods implemented by this server.
   * @param  calldata  parameters, must be none
   * @param  disp      pointer to actual dispatcher
   * @return Array with names of methods
   */
   static MethodResponse system_listMethods(const MethodCall &calldata,
                                            const Dispatcher *disp);

 /** Returns an array of possible signatures for this method.
   * The signature consists of the return value and all parameters
   * divided by commas.
   * @param  calldata  1 parameter with method name.
   * @param  disp      pointer to actual dispatcher
   * @return Array with names of signatures. non-Array ==> no signature available.
   */
   static MethodResponse system_methodSignature(const MethodCall &calldata,
                                                const Dispatcher *disp);

 /** Returns a documentation string describing the use of this method.
   * If appropriate the return string contains html markup. If there are
   * overloaded methods a description for each may be returned.
   * @param  calldata  1 parameter with method name.
   * @param  disp      pointer to actual dispatcher
   * @return RpcString with description
   */
   static MethodResponse system_methodHelp(const MethodCall &calldata,
                                           const Dispatcher *disp);

 /** Returns a struct containing the capabilities of this system.
   * Derived class should re-implement addCapabilities() if appropriate
   * and invode parents accordingly.
   * @param  calldata  0 parameters included
   * @param  disp      pointer to actual dispatcher
   * @return Struct with capabilities.
   */
  static MethodResponse system_getCapabilities(const MethodCall &calldata,
                                               const Dispatcher *disp);

 /** Returns a struct containing the capabilities of this system.
   * @param str  reference to a Struct to return the capabilities
   */
   virtual void getCapabilities (Struct &str) const;

 private:

   MethodCallMap             methodcalls;
   Protocol                 *protocol;
   bool                      wbxml_mode;
};


}  // namespace ulxr


#endif // ULXR_DISPATCHER_H
