/***************************************************************************
               ulxr_valueparse_wb.h  -  parse wbxml-rpc primitive values
                             -------------------
    begin                : Fri Jan 09 2004
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_valueparse_wb.h 1154 2009-08-16 09:24:53Z ewald-arnold $

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

#ifndef ULXR_VALUEPARSE_WB_H
#define ULXR_VALUEPARSE_WB_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <ulxmlrpcpp/ulxr_wbxmlparse.h>
#include <ulxmlrpcpp/ulxr_valueparse_base.h>

#include <stack>


namespace ulxr {


/** Base class for WBXML RPC parsing.
  *
  * IMPORTANT:
  * The current "Value" is moved around via pointers and is not
  * automatically destroyed. The object taking over the "Value" resp. the object
  * storing the value somehow else must "delete" the "Value" it gets.
  *
  * @see ArrayState::takeValue
  * @ingroup grp_ulxr_parser
  */
class ULXR_API_DECL0 ValueParserWb : public ValueParserBase,
                                  public WbXmlParser
{
 public:

 /** Constructs a parser.
   */
   ValueParserWb();

 /** Destroys the parser.
   */
   virtual ~ValueParserWb();

   enum ValueWellKnownToken
   {
     //* @attention Never ever change these values or their order
     wbToken_Value     = wbxml_TAG_C_FIRST,   // 0x45
     wbToken_Array,                           // 0x46
     wbToken_Data,                            // 0x47
     wbToken_Struct,                          // 0x48
     wbToken_Member,                          // 0x49
     wbToken_Name,                            // 0x4a
     wbToken_Boolean,                         // 0x4b
     wbToken_Int,                             // 0x4c
     wbToken_I4,                              // 0x4d
     wbToken_Double,                          // 0x4e
     wbToken_String,                          // 0x4f
     wbToken_Base64,                          // 0x50
     wbToken_Date,                            // 0x51
     wbToken_ValueParserLast                  // 0x52
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

/** Helper class to represent the data of the current parsing step
  * when the xml element is an Integer
  */
  class ULXR_API_DECL0 IntegerState : public ValueParserBase::ValueState
  {
   public:

   /** Constructs a IntegerState.
     * @param  st   the actual state
     */
     IntegerState(unsigned st);

   /** Transfers a Value into the ValueState.
     * @param  val   the value
     * @param  candel: @li true:  value is unique here, delete at end
     *                 @li false: value is shared here, delete it somewhere else
     */
     virtual void takeValue(Value *val, bool candel = true);

   private:
     IntegerState(const IntegerState&); // forbid this
     IntegerState& operator= (const IntegerState&);
  };

  friend class IntegerState;

 /** Gets a pointer to the topmost ValueState.
   * @return pointer to ValueState
   */
   ValueState *getTopValueState() const;
};


}  // namespace ulxr


#endif // ULXR_VALUEPARSE_WB_H
