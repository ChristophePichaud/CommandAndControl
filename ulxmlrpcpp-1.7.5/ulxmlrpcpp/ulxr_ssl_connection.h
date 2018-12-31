/***************************************************************************
                   ulxr_ssl_connection.h  -  ssl connection
                             -------------------
    begin                : Mon May 3 2004
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_ssl_connection.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_SSL_CONNECTION_H
#define ULXR_SSL_CONNECTION_H


#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#ifdef ULXR_INCLUDE_SSL_STUFF

#include <openssl/ssl.h>

#include <ulxmlrpcpp/ulxr_tcpip_connection.h>


namespace ulxr {


/** Class for ssl connections between XML RPC client and server.
  * @ingroup grp_ulxr_connection
  */
class ULXR_API_DECL0 SSLConnection : public TcpIpConnection
{
 public:

 /** Constructs a generic connection, primarily for a client.
   * The connection is not yet open after construction.
   * @param  server  true: bind socket to adress for server mode
   * @param  domain  domain name of the server
   * @param  port    port on the the server
   */
   SSLConnection(bool server, const CppString &domain, unsigned port = 80);

 /** Constructs a generic connection, primarily for a server.
   * The connection is not yet open after construction.
   * @param  server  true: bind socket to adress for server mode
   * @param  adr     accepted client adress
   * @param  port    port on which the connect is accepted
   */
   SSLConnection(bool server, long adr = INADDR_ANY, unsigned port = 0);

 /** Constructs a connection.
   * The connection is not yet open after construction.
   */
   virtual ~SSLConnection();

 /** Closes the connection.
   */
   virtual void close();

 /** Opens the connection in rpc client mode.
   */
   virtual void open();

 /** Opens the connection in rpc server mode, thus waiting for
   * connections from clients.
   * @param timeout the timeout value [sec] (0 - no timeout)
   * @returns <code>true</code> when connection has been accepted
   */
   virtual bool accept(int timeout = 0);

 /** Tests if the current transmission expects a return value.
   * @return true: return value for request
   */
   virtual CppString getInterfaceName();

 /** Cuts the connection.
   * Just the variables are reset, the connection itself is not touched.
   */
   virtual void cut();

 /** Returns the password.
   * @return password
   */
   std::string getPassword() const;

 /** Sets the cryptography data.
   * @param  password   password for the crypto files
   * @param  certfile   name of the servers certificate file (PEM format)
   * @param  keyfile    name of the servers private key file (PEM format)
   */
   void setCryptographyData (const std::string &password,
                             const std::string &certfile,
                             const std::string &keyfile);

 protected:

 /** Returns the SSL object.
   * @return pointer to ssl object
   */
   SSL *getSslObject() const;

 /** Returns the SSL context object.
   * @return pointer to ssl context object
   */
   SSL_CTX *getSslContextObject() const;

 /** Returns the SSL session object.
   * @return pointer to ssl session object
   */
   SSL_SESSION *getSslSessionObject() const;

 /** Checks if there is input data which can immediately be read.
   * @return true: data available
   */
   virtual bool hasPendingInput() const;

 private:

   SSL          *ssl;
   SSL_CTX      *ssl_ctx;
   SSL_SESSION  *session;

   std::string   password;
   std::string   keyfile;
   std::string   certfile;

   static bool ssl_initialized;

 /** Create SSL object.
   */
   void createSSL();

 /** Initialise SSL context data.
   */
   void initializeCTX();

 /** Actually writes data to the connection.
   * @param  buff pointer to data
   * @param  len  valid buffer length
   * @return  result from api write function
   */
   ssize_t virtual low_level_write(char const *buff, long len);

 /** Reads data from the connection.
   * @param  buff pointer to data buffer
   * @param  len  maimum number of bytes to read into buffer
   * @return  result from api read function
   */
   ssize_t virtual low_level_read(char *buff, long len);

 /** Initializes internal variables.
   */
   void init();

  /** Creates a shallow copy of this object.
    * @return pointer to shallow copy
    */
    virtual TcpIpConnection *makeClone();
};


}  // namespace ulxr


#endif // ULXR_INCLUDE_SSL_STUFF


#endif // ULXR_SSL_CONNECTION_H

