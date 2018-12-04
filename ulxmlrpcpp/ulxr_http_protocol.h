/***************************************************************************
               ulxr_http_prootocol.h  -  http prootocol
                             -------------------
    begin                : Mon May 3 2004
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_http_protocol.h 1089 2007-11-10 10:40:43Z ewald-arnold $

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

#ifndef ULXR_HTTP_PROTOCOL_H
#define ULXR_HTTP_PROTOCOL_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <map>
#include <vector>

#include <ulxmlrpcpp/ulxr_protocol.h>


namespace ulxr {


class TcpIpConnection;
class ConnectorWrapperBase;

/** Runs http as protocol for rpc transmition.
  * @ingroup grp_ulxr_protocol
  */
class ULXR_API_DECL0 HttpProtocol : public Protocol
{
 public:

   typedef std::map<CppString, CppString> header_property;

 /** Constructs a Protocol.
   * @param  conn      pointer to connection object
   * @param  hostname  name of host
   * @param  hostport  port of host
   */
   HttpProtocol(Connection *conn, const CppString &hostname, unsigned hostport);

 /** Constructs a Protocol.
   * @param  conn      pointer to connection object
   */
   HttpProtocol(TcpIpConnection *conn);

 /** Destroys the Protocol.
   */
   virtual ~HttpProtocol();

 /** Tests if the response was successful regarding the transportation.
   * @param   phrase  return value describing the problem.
   * @return true  response is OK.
   */
   virtual bool responseStatus(CppString &phrase) const;

 /** Sends a MethodCall over the connection.
   * @param   call      pointer to the calling data
   * @param   resource  resource for rpc on remote host
   * @param   wbxml     true: data is sent as wbxml
   */
   virtual void sendRpcCall(const MethodCall &call, const CppString &resource, bool wbxml);

 /** Sends a MethodResponse over the connection.
   * @param   resp   pointer to the response data
   * @param   wbxml  true: data is sent as wbxml
   */
   virtual void sendRpcResponse(const MethodResponse &resp, bool wbxml);

 /** Resets the state of the Protocol.
   * Before starting a transfer you should call this to ensure
   * a defined state of the internal state machine processing the
   * protocol.
   */
   virtual void resetConnection();

 /** Process a chunk of input data from the connection.
   * Usually you call read() to get some more data and pass it to
   * this method. It processes the header.
   * Normally you only have to take care of two states: ConnError which
   * results in terminating the connection. ConnBody on the other side
   * indicates user data which might be fed to a parser. @see Dispatcher.
   * @param  buffer   pointer to input data
   * @param len       valid length of buffer
   * @return   current state of the processing. This return state may differ from the
   *           state returned by getConnectionState() due to internal processing.
   *           You should ignore everything except ConnBody and ConnError.
   */
   virtual State connectionMachine(char * &buffer, long &len);

 /** Sets the current transmission to expect/accept no return value.
   */
   virtual void setTransmitOnly();

 /** Tests if the current transmission expects a return value.
   * @return true: return value for request
   */
   virtual bool isTransmitOnly();

  /** Returns the protocol name.
   * @return protocol name
   */
   virtual CppString getProtocolName();

 /** Tests if there are bytes left in the message body.
   * @return true: there is at least one byte available
   */
   virtual bool hasBytesToRead() const;

 /** Determines what to do after completing the current transaction.
   * Depending on the protocol version and some properties the connection
   * remains open or closes.
   * @param  http_ver   numeral part of the version from the http header
   * @return true: close connection afterwards
   */
   virtual bool determineClosing(const CppString &http_ver);

 //////////////////////////////////////////////////////////////////////////////////
 /// http stuff

 /** Sends a header and a html body containing error values from "outside".
   * @param  status  http error code
   * @param  phrase  human readable description
   * @param  info    additionaly info to appear in the html response part
   */
   void sendNegativeResponse(int status, const CppString &phrase,
                             const CppString &info = ULXR_PCHAR(""));

 /** Sends a http request header.
   * The actual data must follow directly afterwards.
   * @param  method     the http method in use
   * @param  resource   the requested resource
   * @param  type       the content-type of the requesting data
   * @param  len        the length of the data in the requesting body
   * @param  wbxml_mode true: suppress some header fields
   */
   void sendRequestHeader(const CppString &method,
                          const CppString &resource,
                          const CppString &type, unsigned long len,
                          bool wbxml_mode = false);

 /** Sends a http response header.
   * The actual data must follow directly afterwards.
   * @param  code       http status code
   * @param  phrase     human readable http status phrase
   * @param  type       the content-type of the requesting data
   * @param  len        the length of the data in the requesting body
   * @param  wbxml_mode true: suppress some header fields
   */
   void sendResponseHeader(int code,
                           const CppString &phrase,
                           const CppString &type,
                           unsigned long len,
                           bool wbxml_mode = false);

 /** Gets the user agent.
   * The user agent describes the client application.
   * @return  the user agent.
   */
   CppString getUserAgent() const;

 /** Sets the user agent.
   * The user agent describes the client application.
   * @param  ua  the user agent.
   */
   void setUserAgent(const CppString &ua);

 /** Sends a negative response to the caller
   * @param  realm   Description for the caller which username and password to use
   * @return true: access shall be granted.
   */
   virtual void rejectAuthentication(const CppString &realm);

 /** Sets username and password for the next transmission (client mode).
   * This makes the next transmission block use a simple authentication scheme
   * with a username and password.
   * @param  user   Username
   * @param  pass   Password
   */
   virtual void setMessageAuthentication(const CppString &user, const CppString &pass);

 /** Sets proxy username and password for all the following transmission (client mode).
   * This makes the transmissions use a simple authentication scheme
   * with a username and password. Set both strings empty to disable proxy authentication.
   * @param  user   Username
   * @param  pass   Password
   */
   virtual void setProxyAuthentication(const CppString &user, const CppString &pass);

 /** Gets a property field from the http header.
   * The names are case insensitive. If the property is not available
   * RuntimeException is throw so you should before with hasHttpProperty.
   * @param   name   the name of the field
   * @return  the content of the field.
   */
   CppString getHttpProperty (const CppString &name) const;

 /** Tests if a property field from the http header is available.
   * The names are case insensitive.
   * @param   name   the name of the field
   * @return  true if the property field is available
   */
   bool hasHttpProperty (const CppString &name) const;

 /** Gets the current date as valid string for a http header.
   * @return  date as string
   */
   static CppString getDateStr();

 /** Gets the first line from the http header.
   * the forst line contains special information depending on the
   * direction from or to the server.
   * @return  date as string
   */
   CppString getFirstHeaderLine() const;

 /** Returns a cloned HttpProtocol
   * @return pointer to the cloned object
   */
   virtual HttpProtocol *clone() const;

 /** Detaches the protocol by creating a duplicate of
   * the protocol + connection and closing the original connection afterwards.
   * @return the current protocol
   */
   virtual Protocol *detach();

 /** Clears the current cookie set.
   */
   void clearCookie();

 /** Tests if any cookies have been set
   * @return  true: cookies have been set
   */
   bool hasCookie() const;

 /** Sets the current cookie set.
   * The set comes from both header line "Cookie:" and "Set-Cookie:".
   * @param cont  cookie line from http header
   */
   void setCookie(const CppString &cont);

 /** Gets the current cookie set
   * @return single cookie line suitable for a http header
   */
   CppString getCookie() const;

 /** Sets the current cookie set for a server connection.
   * Adds a "Set-Cookie:" line into the response header.
   * @param cookie  cookie line suitable for http header
   */
   void setServerCookie(const CppString &cookie);

 /** Gets the current cookie set for a server connection
   * @return cookie line suitable for http header
   */
   CppString getServerCookie() const;

 /** Checks if a cookie for the server connection is set.
   * @return cookie is set
   */
   bool hasServerCookie() const;

 /** Sets the current cookie set for a client connection
   * Adds a "Cookie:" line into the response header.
   * @param cookie  cookie line suitable for http header
   */
   void setClientCookie(const CppString &cookie);

 /** Gets the current cookie set for a client connection
   * @return cookie line suitable for http header
   */
   CppString getClientCookie() const;

 /** Checks if a cookie for the client connection is set.
   * @return cookie is set
   */
   bool hasClientCookie() const;

 /** Sets the acceptance of cookies.
   * @param bAccept  true: accept cookies.
   */
   void setAcceptCookies(bool bAccept = true);

 /** Queries the acceptance of cookies.
   * @return true: accept cookies.
   */
   bool isAcceptCookies() const;

 /** Shuts down the socket.
   * @param mode  shutdown mode
   *              @li Unix:  SHUT_RD, SHUT_WR or SHUT_RDWR
   *              @li Win32: SD_RECEIVE, SD_SEND or SD_BOTH
   */
   virtual void shutdown(int mode);

 /** Closes the connection.
   */
   virtual void close();

 /** Enables the connection via a proxy with CONNECT method
   * @param  enable  true: enable CONNECT method
   */
   void enableConnect(bool enable = true);

 /** Checks if the http CONNECT is to be used.
   * @return true: use CONNECT via proxy
   */
   bool isConnectEnabled() const;

 /** Checks if the http CONNECT has succeeded.
   * @return true: CONNECT has suceeded
   */
   bool isConnected() const;

 /** Sends a CONNECT message to the proxy and waits for the response.
   */
   void doConnect();

 /** Splits the first header line in a http response into its three parts.
   * @param   head_version  [OUT]  http version
   * @param   head_status   [OUT]  http response status
   * @param   head_phrase   [OUT]  an explanation phrase
   */
   void splitHeaderLine(CppString &head_version, unsigned &head_status, CppString &head_phrase);

 /** Writes the body to the data stream. The current encoding is honoured.
   * @param data    pointer to the data
   * @param len     length of the data
   */
   void writeBody(const char *data, unsigned long len);

 /** Writes a chunk to the data stream. The chunk size must be != 0.
   * @param data    pointer to the data
   * @param len     length of the data
   */
   void writeChunk(const char *data, unsigned long len);

 /** Sets the chunk size for chunked encoding.
   * @param chunked   false == no chunked transfer
   */
   void setChunkedTransfer(bool chunked);

 /** Gets the chunk size for chunked encoding.
   * @return maximum false == no chunked transfer
   */
   bool isChunkedTransfer() const;

protected:

 /** Http connection states while reading input stream
   */
   typedef enum
   {
     ConnChunkHeader = Protocol::ConnLast,   // 7 chunk header of a chunked transfer
     ConnChunkBody,                          // 8 data chunk of a chunked transfer
     ConnChunkBodySkip,                      // 9 skip CRLF after chunk body
     ConnChunkTerminated                     // 10 all chunks and footer received
   }
   HttpState;

 /** Tries to connect to the outside server via a proxy with http CONNECT
   */
   void tryConnect();

 /** Waits for the proxy response about the result of the CONNECT
   */
   void awaitConnect();

 /** Checks if the current http has contains some closing property.
   * This recognises "Connection: Close" as well as "Proxy-Connection: Close".
   * @return true: Closing property is included
   */
   bool hasClosingProperty();

 /** Reset all information contained in the http header.
   */
   void clearHttpInfo();

 /** Extracts username and password from current message.
   * @param  user   reference to return username
   * @param  pass   reference to return password
   * @return true: username and password could be extracted
   */
   virtual bool getUserPass(CppString &user, CppString &pass) const;

 /** Adds a http header field for the next transmission.
   * The user fields are reset after transmission.
   * @param name     http header field name
   * @param value    value of header field
   */
   void addOneTimeHttpField(const CppString &name, const CppString &value);

 /** Determines the length of the message body.
   */
   virtual void determineContentLength();

 /** Parses the first line of the header.
   * The first line contains special information like path to the
   * data, http version in use or error information.
   */
   void parseHeaderLine();

 /** Checks for a pending 100-Continue.
   */
   bool checkContinue();

 private:

 /** Initializes internal variables.
   */
   void init ();

 /** Statemachine switches to body.
   * @param  buffer       pointer to input data
   * @param len           valid length of buffer
   * @param  chunk_start  pointer to start of chunk data
   * @param  chunk_cursor pointer to current chunk data
   */
   void machine_switchToBody(char * &buffer,
                             long &len,
                             char * &chunk_start,
                             char * &chunk_cursor);

 protected:
   HttpProtocol(const HttpProtocol&);
 private:
   HttpProtocol& operator=(const HttpProtocol&);  // empty!

 private:

   struct PImpl;
   PImpl *pimpl;
};


};  // namespace ulxr


#endif // ULXR_HTTP_PROTOCOL_H
