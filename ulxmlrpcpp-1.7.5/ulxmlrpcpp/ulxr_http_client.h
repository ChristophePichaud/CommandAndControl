/***************************************************************************
                    ulxr_http_client.h  -  a simple http client
                             -------------------
    begin                : Sam Apr 20 2002
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_http_client.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_HTTP_CLIENT_H
#define ULXR_HTTP_CLIENT_H

#ifndef ULXR_OMIT_TCP_STUFF

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header


namespace ulxr {


class HttpProtocol;


/** Helper class to process a chunk of the message body.
  * The default implementation does just nothing. To do something usefully
  * create a subclass and override "process()".
  */
class ULXR_API_DECL0 BodyProcessor
{
 public:

 /** Process a chunk of the message body somehow.
   * @param  buffer pointer to the current chunk
   * @param  len    valid length of the buffer
   */
   virtual void process(const char *buffer, unsigned len);

 /** Destroy the processor.
   * Ensure virtual destructor.
   */
   virtual ~BodyProcessor();
};


/** A simple client for an http connection.
  * This implementation only honors the basic http methods PUT, GET, POST and DELETE.
  * @ingroup grp_ulxr_http
  */
class ULXR_API_DECL0 HttpClient
{
 public:

 /** Constructs a http client.
   * @param  prot  pointer to an existing connection
   */
   HttpClient(HttpProtocol* prot);

 /** Destroys the http client
   */
   virtual ~HttpClient()
     {}

 /** Puts content into a remote resource.
   * @param  msg       content to send via http PUT
   * @param  resource  name of the remote resource
   * @param  type      the mime type of the content (e.g. text/html)
   */
   void msgPUT(const Cpp8BitString &msg,
               const CppString &type,
               const CppString &resource);

 /** Puts content into a remote resource.
   * @param  filename  file to send via http PUT
   * @param  type      the mime type of the content (e.g. text/html)
   * @param  resource  name of the remote resource
   */
   void filePUT(const CppString &filename,
                const CppString &type,
                const CppString &resource);

 /** Gets content from a remote resource.
   * @param  resource  name of the remote resource
   * @return content to receive via http GET
   */
   Cpp8BitString msgGET(const CppString &resource);

 /** Gets content from a remote resource.
   * @param  filename  filename to store the result from the http GET
   * @param  resource  name of the remote resource
   */
   void fileGET(const CppString &filename, const CppString &resource);

 /** Gets context dependent content from a remote resource.
   * Usually this is used in web forms where user data is processed.
   * @param  msg       "selector" for the content to receive via http POST
   * @param  type      the mime type of the content (e.g. text/html)
   * @param  resource  name of the remote resource
   * @return  resulting content
   */
   Cpp8BitString msgPOST(const Cpp8BitString &msg,
                         const CppString &type,
                         const CppString &resource);

 /** Delete a remote resource.
   * @param  resource  name of the remote resource
   */
   void doDELETE(const CppString &resource);

 /** Sets username and password for the following transmissions.
   * This makes the next and all following transmission blocks
   * use a simple authentication scheme with a username and password.
   * @param  user   Username
   * @param  pass   Password
   */
   virtual void setMessageAuthentication(const CppString &user, const CppString &pass);

 protected:

 /** Sends the current authentiocation data over the conenction.
   */
   void sendAuthentication();

 /** Gets the status value from the response.
   * @return status value
   */
   int getHttpStatus() const;

 /** Gets the status phrase from the response.
   * @return status phrase
   */
   CppString getHttpPhrase() const;

 /** Gets the version of the http protocol.
   * This method return just the numeric part after "/". For example
   * "1.1" instead of "HTTP/1.1".
   * @return version
   */
   CppString getHttpVersion() const;

 /** Processes all the http headers after the header has been completely
   * read.
   */
   virtual void interpreteHttpHeader();

 /** Receives a complete response from the server.
   * @param proc pointer to a helper instance to process the message body.
   */
   void receiveResponse(BodyProcessor &proc);

 private:

   unsigned        head_status;
   CppString       head_phrase;
   CppString       head_version;
   CppString       http_user;
   CppString       http_pass;

   HttpProtocol   *protocol;
};


}  // namespace ulxr


#endif // ULXR_OMIT_TCP_STUFF

#endif // ULXR_HTTP_CLIENT_H

