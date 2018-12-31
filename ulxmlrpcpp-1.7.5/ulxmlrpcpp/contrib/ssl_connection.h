/***************************************************************************
       ssl_connection.h  -  ssl connection with certificate support
                             -------------------
    begin                : Sun May 29 2005
    copyright            : (C) 2005 Dmitry Nizovtsev <funt@alarit.com>
                                    Olexander Shtepa <isk@alarit.com>

    $Id: ssl_connection.h 945 2007-01-03 13:05:48Z ewald-arnold $

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

#ifndef FUNTIK_SSL_CONNECTION_H
#define FUNTIK_SSL_CONNECTION_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#ifdef ULXR_INCLUDE_SSL_STUFF

#include <ulxmlrpcpp/ulxr_value.h>
#include <ulxmlrpcpp/ulxr_tcpip_connection.h>
#include <ulxmlrpcpp/ulxr_except.h>

#include <openssl/ssl.h>

//STL
#include <string>


#define		FINGERPRINT_MAX_LEN	1024

namespace funtik {

class ULXR_API_DECL0 SSLConnectionException : public ulxr::ConnectionException
{
public:

    SSLConnectionException();
    SSLConnectionException(ulxr::CppString strPhrase,int stat);
	SSLConnectionException(SSL *ssl, int ret_code);
	virtual ~SSLConnectionException() throw() { };


 /** Gets the reason for the exception.
   * @return   the reason
   */
	virtual ulxr::CppString why() const;



 /** Gets the reason for the exception(STL).
   * @return   the reason
   */
	virtual const char *what() const throw();
protected:
	std::string _what;

	std::string get_error_queue();
};


 /**
  * @brief The class for support ssl connections.
  *
  * This support SSL connections for server and client.
  * This support SSL authentification,check certificates, etc.
  *
  * @ingroup grp_ulxr_connection
  */

class ULXR_API_DECL0 SSLConnection : public ulxr::TcpIpConnection
{
 public:

 /** Constructs a generic connection, primarily for a client.
   * The connection is not yet open after construction.
   * @param  server  true: bind socket to adress for server mode
   * @param  domain  domain name of the server
   * @param  port    port on the the server
   */

   SSLConnection(bool server, const ulxr::CppString &domain, unsigned port = 80);

 /** Constructs a generic connection, primarily for a server.
   * The connection is not yet open after construction.
   * @param  server  true: bind socket to adress for server mode
   * @param  adr     accepted client adress
   * @param  port    port on which the connect is accepted
   */

  SSLConnection(bool server, long adr = INADDR_ANY, unsigned port = 0);


  // Specifies a pathname for the file containing the trusted certificates,
  // which are stored sequentially in PEM format. In most applications,
  // the client uses this function to authenticate the server. If client authentication is desired,
  // the server also calls this function to authenticate the client.


 /** Constructs a connection.
   * The connection is not yet open after construction.
   */
   virtual ~SSLConnection ();

 /** Closes the connection.
   */
   virtual void close();

 /** Detaches the connection by creating a duplicate of
   * the connection and closing the original connection afterwards.
   * @return the current connection
   */
   virtual ulxr::Connection *detach();

 /** Opens the connection in rpc client mode.
   */
   virtual void open();

 /** Opens the connection in rpc server mode, thus waiting for
   * connections from clients.
   */
   virtual void accept();

 /** Tests if the current transmission expects a return value.
   * @return true: return value for request
   */
   virtual ulxr::CppString getInterfaceName();

 /**
   * Cuts the connection.
   * Just the variables are reset,creted new ssl_ctx, the connection itself is not touched.
   */
   virtual void cut();


 /** Sets the cryptography data.
   * @param  password   password for the crypto files
   * @param  certfile   name of the servers or clients certificate file (PEM format)
   * @param  keyfile    name of the servers or clients private key file (PEM format)
   */
   void setCryptographyData (const std::string &password,
                             const std::string &certfile,
                             const std::string &keyfile);

 /**
  * Set the password for cryptography client`s certificate.
  * @param	strPassword password for the crypto files.
  */
	void setPassword(const std::string &strPassword);

 /**
  * @brief Get the password.
  *
  * Returns the password for key.
  *
  * @return password
  */
   	std::string getPassword() const;

 /**
  * @brief set the certificate.
  *
  * Set the name of the servers or clients certificate file (PEM format).
  * Function will be generate exception if certificate have bad format.
  *
  * @param	strCertFile name of the certificate file
  */
	void setCertFile(const std::string &strCertFile);

 /**
  * @brief get name of certificate.
  *
  * Get the name of the servers or clients certificate file.
  *
  * @return name of the certificate file.
  */
	std::string getCertFileName() const;

	void setKeyFile(const std::string &strCertFileName);


 /**
  * @brief get name of key.
  *
  * Get the name of the servers or clients key file.
  *
  * @return name of the key file.
  */
	std::string getKeyFileName() const;


 /**
  * @brief Sets the CA file data.
  *
  * @param  strCAFile  name of file contains client`s certificates
  */
	void setCAFile(const std::string &strCAFile);

 /**
  * @brief Enable SSL authentificate.
  *
  * Enable SSL authentificate. Maybe two types of authentificate.
  * CA_AUTH all signed certificates is validate(need call function for set CA file).
  * FINGERPRINT_AUTH certificate is validate if server or client has one fingerprint.
  *
  * Note:
  * if need
  *
  * @param iType type of authentificate.
  * @sa
  */
	void enableAuth(int iType);

 /**
  * @brief Disable SSL authentificate.
  *
  * Disable SSL authentificate options.
  *
  * @param iType type of disabled authentificate. if iType is  0 or absent all authentificate set disable.
  */
	void disableAuth(int iType=0);


	enum AuthType{
		CA_AUTH=1,
		FINGERPRINT_AUTH=2,
		MANDATORY_CLIENTCERT=4,
		CHECK_REVOCATIONCERT=8
	};

 /**
  *  Adds the  fingerprint of client`s certificate.
  *  For Generating fingerpint for certificate certificate.pem use next command:
  * 	openssl x509 -fingerprint -in ./certificate.pem
  * @param strFingerprint filename with fingerprints.
  */
	void addFingerprintData(const std::string  &strFingerprint);

 /**
  * Reset all fingerprint data of client`s certificate.
  *
  */
	void resetFingerprintData();

 /**
  * Adds the fingerprint of revocation client`s certificate.
  *
  * @param strRevocation filename with revocation fingerprints.
  */
	void addRevocationData(const std::string &strRevocation);

 /**
  * Reset all fingerprint data of revocation certificates.
  *
  */
	void resetRevocationData();

 /**
  * Check fingerprint in fingerprint data.
  *
  * @param  strFingerprint
  * @return true if fingerprint of certificate present, else false.
  */
	bool checkFingerprint(std::string strFingerprint) const;


 /**
  * Check fingerprint in revocation fingerprint data.
  *
  * @param  strFingerprint
  * @return true if fingerprint of certificate present, else false.
  */
	bool checkRevocationFingerprint(std::string strFingerprint) const;

 /**
  * Calculates certificate fingerprint.
  * @return fingerprint of certificate.
  */
  std::string calculateFingerprint(X509 * poCertificate) const;

 protected:

 /**
  * if m_CA_auth is true then used authentification by means of CA certificate.
  * Path to the CA certificates must be in variable m_CA_file.
  */
  	int 		m_iAuthType;
	std::string	m_strCAFileName;

	std::map<std::string,int>	m_mapFingerpintData;
	std::map<std::string,int>	m_mapRevocationData;

	std::string	m_strPassword;
	std::string	m_strKeyFileName;
	std::string	m_strCertFileName;


 /**
  * Check certificate in fingerprint data and revocation data.
  * This is used by accept and open functions.
  *
  * @return true if access allow, else false.
  */
	bool checkAccess(X509 * poCertificate);



 /**
  * @brief activate security functions.
  *
  * Active security.
  * Used in enableAuth and disableAuth functions.
  *
  */
    void activateAuth();


 /**
  *  Returns the SSL object.
  * @return pointer to ssl object
  */
   SSL *getSslObject() const;


 /**
  *  Returns the SSL context object.
  * @return pointer to ssl context object
  */
   SSL_CTX *getSslContextObject() const;

 /**
  *  Returns the SSL session object.
  * @return pointer to ssl session object
  */
   SSL_SESSION *getSslSessionObject() const;

 /**
  *  Checks if there is input data which can immediately be read.
  * @return true: data available
  */
   virtual bool hasPendingInput() const;

   SSL          *ssl;
   SSL_CTX      *ssl_ctx;
   SSL_SESSION  *session;

   static bool ssl_initialized;

 /** Create SSL object.
   */
   void createSSL();

 /** Initialise SSL context data.
   */
   void initializeCTX();

 /**
  *  Actually writes data to the connection.
  * @param  buff pointer to data
  * @param  len  valid buffer length
  * @return  result from api write function
  */
   ssize_t virtual low_level_write(char const *buff, long len);

 /**
  *  Reads data from the connection.
  * @param  buff pointer to data buffer
  * @param  len  maimum number of bytes to read into buffer
  * @return  result from api read function
  */
   ssize_t virtual low_level_read(char *buff, long len);

 /**
  *  Initializes internal variables.
  */
   void init();
};


} // namespace funtik


#endif // ULXR_INCLUDE_SSL_STUFF


#endif // FUNTIK_SSL_CONNECTION_H

