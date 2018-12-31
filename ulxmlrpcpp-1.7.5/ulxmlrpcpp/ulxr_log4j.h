/***************************************************************************
               ulxr_log4j.h  -  logging based on log4j
                      -------------------
    begin                : Fri Jul 13 2007
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_log4j.h 1164 2010-01-06 10:03:51Z ewald-arnold $

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

#ifndef ULXR_LOG4J_H
#define ULXR_LOG4J_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#ifndef ULXR_OMIT_TCP_STUFF

namespace ulxr {

class TcpIpConnection;


/** Simple logger support class based on log4j.
  */
class ULXR_API_DECL0 Log4JSender
{
 public:

 /** Creates the Log4JSender object.
   * @param appname   Name and possibly path of this application
   * @param conn      Pointer to a prepared connection
   */
   Log4JSender(const CppString &appname, TcpIpConnection &conn);

 /** Destructs Log4JSender object.
   */
   ~Log4JSender() throw();

 /** Opens the connection
   */
   void open();

 /** Closes the connection
   */
   void close();

 /** Sends a message to the log host
   * @param level     debug Level
   * @param message   debug message
   * @param filename  filename of message origin
   * @param line      line number in filename
   */
   void send(const CppString &level, const CppString &message,
             const CppString &filename = ULXR_PCHAR(""), unsigned line = 0);

  /** The number of milliseconds elapsed from 1/1/1970 until logging event
    * was created.
    * @return  current value of milliseconds
    */
    ULXR_INT64 currentTimeMillis();

  private:

    CppString        appname;
    TcpIpConnection &conn;
    bool             network_error;
    CppString        threadname;
    unsigned long    seqnum;
    std::string      hostname;
    bool             disable_send;
    CppString        classname;
    CppString        methodname;
    bool             first_log4j;
};


}  // namespace ulxr

#endif // ULXR_OMIT_TCP_STUFF

#endif // ULXR_LOG4J_H
