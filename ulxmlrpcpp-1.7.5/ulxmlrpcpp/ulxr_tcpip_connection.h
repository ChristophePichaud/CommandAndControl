/***************************************************************************
               ulxr_tcpip_connection.h  -  tcpip connection

    begin                : Sun Apr 29 2002
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_tcpip_connection.h 1087 2007-11-05 20:10:39Z ewald-arnold $

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

#ifndef ULXR_TCPIP_CONNECTION_H
#define ULXR_TCPIP_CONNECTION_H

#ifndef ULXR_OMIT_TCP_STUFF

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#if defined(__WIN32__) || defined(_MSC_VER)
#include <winsock2.h>
//#include <windows.h>
typedef int socklen_t;
#endif

#ifdef __unix__
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include <ulxmlrpcpp/ulxr_connection.h>


namespace ulxr {


/** Run a generic tcp/ip connection between a client and a server.
  * @ingroup grp_ulxr_connection
  */
class ULXR_API_DECL0 TcpIpConnection : public Connection
{
 public:

 /** Constructs a generic connection, primarily for a client.
   * The connection is not yet open after construction.
   * @param  server  true: bind socket to adress for server mode
   * @param  domain  domain name of the server
   * @param  port    port on the the server. (If port==0 the system might choose
   *                 a free port. Retrievable with \c getPort())
   */
   TcpIpConnection(bool server, const CppString &domain, unsigned port = 80);

 /** Sets a proxy for further connections.
   * This setting only applies for client connections.
   * @param  adr     adress for proxy
   * @param  port    port on which to connect to the proxy
   */
   void setProxy(long adr, unsigned port);

 /** Sets the proxy server data.
   * @param  domain  name for proxy
   * @param  port    port on which to connect to the proxy
   */
   void setProxy(const CppString &domain, unsigned port);

 /** Constructs a generic connection, primarily for a server.
   * The connection is not yet open after construction.
   * @param  server  true: bind socket to adress for server mode
   * @param  adr     accepted client adress
   * @param  port    port on which the connect is accepted
   */
   TcpIpConnection(bool server, long adr = INADDR_ANY,  unsigned port = 0);

 /** Destroys the connection.
   */
   virtual ~TcpIpConnection();

 /** Detaches the connection by creating a duplicate of
   * the connection and closing the original connection afterwards.
   * @return the current connection
   */
   virtual Connection *detach();

 /** Opens the connection in client mode.
   */
   virtual void open();

 /** Opens the connection in server mode, thus waiting for
   * connections from clients.
   * @param timeout the timeout value [sec] for incoming data (0 - no timeout)
   * @returns <code>true</code> when connection has been accepted
   */
   virtual bool accept(int timeout = 0);

 /** Closes the connection.
   */
   virtual void close();

 /** Shuts down the socket.
   * @param mode  shutdown mode
   *              @li Unix:  SHUT_RD, SHUT_WR or SHUT_RDWR
   *              @li Win32: SD_RECEIVE, SD_SEND or SD_BOTH
   */
   virtual void shutdown(int mode);

 /**
   * Returns the configured port.
   */
   unsigned getPort();

 /** Gets the host name.
   * @return  the host name.
   */
   CppString getHostName() const;

 /** Gets the server doamin.
   * @return  the server domain.
   */
   CppString getServerDomain() const;

 /** Gets the name of the remote machine.
   * attention Does not seem to work on all plattforms. Search source file for
   * \c ULXR_ENABLE_GET_PEERNAME on how to enable this features.
   * @return  the remote name.
   */
   CppString getPeerName() const;

 /** Converts ascii text to in_addr struct.
   * @param address  literal string of address to convert
   * @param saddr    return struct with data
   */
   void asciiToInAddr(const char *address,
                      struct in_addr &saddr);

 /** Sets abort on close flag for a socket. Timeout value comes from \c getTimeout(),
   * @param bOn    sets SO_LINGER state of the socket on/off.
   * @return status of setsockopt
   * See -s 3SOCKET setsockopt and sys/socket.h for details.
   */
   int abortOnClose(int bOn);

 /** Sets the socket buffer mechanism.
   * By disabling the buffer mechanism all data is sent immediately.
   * This may result in higher performance especially with persistent
   * connections but will also increase network traffic with more but
   * smaller packets. Use with care.
   * @param bOn sets the TCP_NODELAY option on the socket
   */
   void setTcpNoDelay(bool bOn);

  /** Gets a readable name for this connection type.
   * @return name of connection type
   */
   virtual CppString getInterfaceName();

 /** Portable function to return the current socket error number.
   * @return error number (errno under Unices)
   */
   virtual int getLastError();

 /** Checks if the connection is run as server.
   * @return true: server mode
   */
    bool isServerMode() const;

 /** Returns the server connection handle.
   * @return file handle
   */
    virtual int getServerHandle();

 protected:

  /** Creates a \c hostent struct from a host name.
    * @return pointer to \c hostent structure
    */
    struct hostent *getHostAdress(const CppString &hostname);

  /** Creates a shallow copy of this object.
    * @return pointer to shallow copy
    */
    virtual TcpIpConnection *makeClone();

 /** Decrements the reference count to the server data.
   * If the count reaches 0 the server is deleted.
   * @param shutdown true: shutdown the socket
   */
   virtual void decrementServerRef(bool shutdown = false);

   class ServerSocketData;

 /** Sets the pointer to the server-data.
   * The reference count to the previous server is decremented.
   * @param serv_data   pointer to server
   */
   void setServerData (ServerSocketData *serv_data);

 /** Gets the pointer to the server-data.
   * @return   pointer to server
   */
   ServerSocketData *getServerData () const;

 private:

 /** Initializes internal variables.
   * @param   port   the port used for the connection
   */
   void init (unsigned port);

 /** Sets the socket buffer mechanism.
   * @return status of setsockopt
   */
   int doTcpNoDelay();

 protected:

  TcpIpConnection(const TcpIpConnection&);

 private:

   // forbid them all due to internal pointers
  TcpIpConnection& operator=(const TcpIpConnection&); // empty!

 private:

   struct PImpl;
   PImpl *pimpl;
   int    noDelayOpt;
};


/** Helper class to handle the server socket.
  */
class ULXR_API_DECL0 TcpIpConnection::ServerSocketData
{
  public:

  /** Construct the server socket.
    * @param  s_no  the socket handle
    */
    ServerSocketData(int s_no);

  /** Destroy the server socket.
    */
    virtual ~ServerSocketData();

  /** Gets the socket handle.
    * @return  the socket handle
    */
    int getSocket() const;

  /** Closes the socket - no shutdown.
    */
    void close();

  /** Shuts down the socket.
   * @param mode  shutdown mode
   *              @li Unix:  SHUT_RD, SHUT_WR or SHUT_RDWR
   *              @li Win32: SD_RECEIVE, SD_SEND or SD_BOTH
    */
    virtual void shutdown(int mode);

  /** Queries if the socket has been closed via \c close() or \c shutdown().
    */
    bool isOpen();

  /** Increment the usage count by 1.
    */
    void incRef();

  /** Decrement the usage count by 1.
    * @return the current usage count after decrementing.
    */
    int decRef();

  private:

    int    count;
    int    socket_no;
};


}  // namespace ulxr


#endif // ULXR_OMIT_TCP_STUFF

#endif // ULXR_TCPIP_CONNECTION_H
