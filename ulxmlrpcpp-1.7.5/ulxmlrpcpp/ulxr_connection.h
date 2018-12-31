/***************************************************************************
           ulxr_connection.h  -  provide a connection for rpc-data
                             -------------------
    begin                : Sun Mar 24 2002
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_connection.h 1062 2007-08-19 09:07:58Z ewald-arnold $

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

#ifndef ULXR_CONNECTION_H
#define ULXR_CONNECTION_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#if defined(__BORLANDC__)  || defined (_MSC_VER)
typedef long ssize_t; // TODO detect
#endif

namespace ulxr {

/** @brief A connection object to transport XML-RPC calls.
  * @defgroup grp_ulxr_connection A connection for an XML-RPC call
  */

/** Internal helper class, not intended for public use.
 * @internal
 */
class ULXR_API_DECL0 ConnectorWrapperBase
{
  public:

    virtual ~ConnectorWrapperBase();

    virtual void call() = 0;
};


/** Internal helper class template, not intended for public use.
 * @internal
 */
template <class T>
class ULXR_API_DECL0 ConnectorWrapper : public ConnectorWrapperBase
{
  public:

   typedef void (T::*PMF)();

   virtual void call ()
   {
      (obj->*adr) ();
   }

   ConnectorWrapper(T *o, PMF a)
     : obj(o), adr(a)
   {}

  private:

   T    *obj;
   PMF   adr ;

 private:
    // forbid them all due to internal pointers
    const ConnectorWrapper& operator= (const ConnectorWrapper&);
    ConnectorWrapper (const ConnectorWrapper&);
};


/** Base class for connection between XML RPC client and server.
  * @ingroup grp_ulxr_connection
  */
class ULXR_API_DECL0 Connection
{
 public:

 /** Constructs a connection.
   * The connection is not yet open after construction.
   */
   Connection();

 /** Destroys the connection.
   * The connection is closed now at the latest.
   */
   virtual ~Connection();

 /** Closes the connection.
   */
   virtual void close();

 /** Shuts down the socket.
   * Only meaningful for certain connections based on TcpIpConnection.
   * @param mode  shutdown mode
   *              @li Unix:  SHUT_RD, SHUT_WR or SHUT_RDWR
   *              @li Win32: SD_RECEIVE, SD_SEND or SD_BOTH
   */
   virtual void shutdown(int mode) = 0;

 /** Detaches the connection by creating a duplicate of
   * the connection and closing the original connection afterwards.
   * @return the current connection
   */
   virtual Connection *detach() = 0;

 /** Writes data to the connection.
   * @param  buff pointer to data
   * @param  len  valid buffer length
   */
   virtual void write(char const *buff, long len);

 /** Reads data from the connection.
   * @param  buff pointer to data buffer
   * @param  len  maimum number of bytes to read into buffer
   * @return number of actually read bytes
   */
   virtual ssize_t read(char *buff, long len);

 /** Opens the connection in rpc client mode.
   */
   virtual void open() = 0;

 /** Opens the connection in rpc server mode, thus waiting for
   * connections from clients.
   * @param timeout the timeout value [sec] (0 - no timeout)
   * @returns <code>true</code> when connection has been accepted
   */
   virtual bool accept(int timeout = 0) = 0;

 /** Tests if the connection is open.
   * @return true if connection is already open.
   */
   virtual bool isOpen() const;

 /** Sets the maximum time it is waited to complete an action like open or read.
   * @param  to_sec  time in seconds
   */
   void setTimeout(unsigned to_sec);

 /** Stores the maximum times for connections with a single transmission a or
   * an alive connection between multiple transmissions. This call only serves
   * as a store for the values. Protocols an other clients must retrieve the according
   * values via \c getDefaultTimeout() and \c getPersistentTimeout() and actually set
   * with \c setTimeout().
   *
   * Values of 0 implicitly select the current timeout value.
   *
   * @param  def_to_sec    default time in seconds. A value of 0 disables the timeout.
   * @param  alive_to_sec  time while a connection shall remain alive between
   *                       transmissions.
   */
   void setConnectionTimeout(unsigned def_to_sec = 0, unsigned alive_to_sec = 0);

 /** Gets the maximum time it is waited to complete an action like open or read.
   * @return time in seconds
   */
   unsigned getTimeout() const;

 /** Gets the default timeout for single transmission connections.
   * @return time in seconds
   */
   unsigned getDefaultTimeout() const;

 /** Gets the alive timeout for alive connections for multiple transactions.
   * @return time in seconds
   */
   unsigned getPersistentTimeout() const;

  /** Portable function to return the current error number.
   * @return error number (errno under Unices)
   */
   virtual int getLastError();

  /** Portable function to return the error description for a given error number
   * @param  err_number    system dependent error code
   * @return error string
   */
   virtual CppString getErrorString(int err_number);

 /** Tests if the current transmission expects a return value.
   * @return true: return value for request
   */
   virtual CppString getInterfaceName() = 0;

 /** Cuts the connection.
   * Just the variables are reset, the connection itself is not touched.
   */
   virtual void cut();

 /** Returns the file handle of the connection.
   * @return  the file handle
   */
   int getHandle() const;

 /** Returns the server connection handle.
   * @return file handle
   */
    virtual int getServerHandle() = 0;

 /** Connect to a proxy or firewall after creating the socket.
   */
   void doConnect();

 /** Sets the callback to the proxy connector.
   * @param  connector   connector callback
   */
   void setConnector(ConnectorWrapperBase *connector);

 protected:

 /** Sets the file handle of the connection.
   * @param handle  the file handle
   */
   void setHandle(int handle);

 /** Checks if the http CONNECT is in progress.
   * @return true: CONNECT is running
   */
   bool isConnecting() const;

 /** Reflects the state when the http CONNECT is in progress.
   * In this case encryption must temporarily be turned off to authenticate with a firewall.
   * @param connecting true: CONNECT is running
   */
   void setIsConnecting(bool connecting);

 /** Actually writes data to the connection.
   * @param  buff pointer to data
   * @param  len  valid buffer length
   * @return  result from api write function
   */
   virtual ssize_t low_level_write(char const *buff, long len);

 /** Reads data from the connection.
   * @param  buff pointer to data buffer
   * @param  len  maimum number of bytes to read into buffer
   * @return  result from api read function
   */
   virtual ssize_t low_level_read(char *buff, long len);

 /** Checks if there is input dta which can innediately be read.
   * @return true: data available
   */
   virtual bool hasPendingInput() const;

 private:

 /** Initializes internal variables.
   */
   void init();

 private:

   bool                   isconnecting;
   ConnectorWrapperBase  *connector;
   int                    fd_handle;

   unsigned               current_to;
   unsigned               default_to;
   unsigned               persist_to;

 private:

    // forbid them all due to internal pointers
//    const ConnectorWrapper& operator= (const ConnectorWrapper&);
//    ConnectorWrapper (const ConnectorWrapper&);
};


}  // namespace ulxr


#endif // ULXR_CONNECTION_H
