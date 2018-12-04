/***************************************************************************
            ulxr_responseparse_base.h  -  parse rpc method response
                             -------------------
    begin                : Fri Jan 09 2004
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_responseparse_base.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_RESPONSEPARSE_BASE_H
#define ULXR_RESPONSEPARSE_BASE_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <ulxmlrpcpp/ulxr_response.h>
#include <ulxmlrpcpp/ulxr_value.h>
#include <ulxmlrpcpp/ulxr_valueparse_base.h>


namespace ulxr {


/** An xml parser for a MethodResponse.
  * @ingroup grp_ulxr_parser
  */
class ULXR_API_DECL0 MethodResponseParserBase
{
 public:

 /** Gets the complete MethodResponse after parsing.
   * @return  the method response
   */
  MethodResponse getMethodResponse();

  enum RespState
  {
    eMethodResponse = ValueParserBase::eValueParserLast,
    eFault,
    eParams,
    eParam,
    eResponseParserLast
  };

 /** Destroy parser.
   */
   virtual ~MethodResponseParserBase();

 protected:

    Value   method_value;
};


}  // namespace ulxr


#endif // ULXR_RESPONSEPARSE_BASE_H
