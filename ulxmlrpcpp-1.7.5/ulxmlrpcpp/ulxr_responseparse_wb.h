/***************************************************************************
            ulxr_responseparse_wb.h  -  parse wbxml-rpc method response
                             -------------------
    begin                : Fri Jan 09 2004
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_responseparse_wb.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_RESPONSEPARSE_WB_H
#define ULXR_RESPONSEPARSE_WB_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <ulxmlrpcpp/ulxr_valueparse_wb.h>
#include <ulxmlrpcpp/ulxr_responseparse_base.h>


namespace ulxr {


/** An xml parser for a MethodResponse.
  * @ingroup grp_ulxr_parser
  */
class ULXR_API_DECL0 MethodResponseParserWb : public ValueParserWb,
                                           public MethodResponseParserBase
{
 public:

   enum RespWellKnownToken
   {
     wbToken_MethodResponse = ValueParserWb::wbToken_ValueParserLast, // 0x52
     wbToken_Fault,                                                   // 0x53
     wbToken_Params,                                                  // 0x54
     wbToken_Param,                                                   // 0x55
     wbToken_ResponseParserLast                                       // 0x56
   };

 protected:

 /** Tests if the current opening tag is to be parsed by this
   * inheritance level or by the parent.
   * @param  token  current well known token
   * @param  attr   tag attributes
   * @return true: element has been handled
   */
   bool testStartElement(unsigned token, const Attributes &attr);

 /** C++ callback for an opening XML tag.
   * @param  token  current well known token
   * @param  attr   tag attributes
   */
   virtual void startElement(unsigned token, const Attributes &attr);

 /** C++ callback for a closing XML tag.
   * @return true: element has been handled
   */
   bool testEndElement();

 /** C++ callback for a closing XML tag.
   */
   virtual void endElement();

};


}  // namespace ulxr


#endif // ULXR_RESPONSEPARSE_WB_H
