/***************************************************************************
               ulxr_valueparse_base.h  -  parse rpc primitive values
                             -------------------
    begin                : Fri Jan 09 2004
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_valueparse_base.h 1154 2009-08-16 09:24:53Z ewald-arnold $

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

#ifndef ULXR_VALUEPARSE_BASE_H
#define ULXR_VALUEPARSE_BASE_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <ulxmlrpcpp/ulxr_value.h>
#include <ulxmlrpcpp/ulxr_xmlparse_base.h>


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
class ULXR_API_DECL0 ValueParserBase
{
 public:

  /** Destroy parser.
   */
   virtual ~ValueParserBase();

  /** Gets the final Value after parsing.
   * @return the Value.
   */
   Value getValue() const;

   enum State
   {
     eValue  = XmlParserBase::eXmlParserLast,
     eArray,   eData,
     eStruct,  eMember, eName,
     eBoolean, eInt,    eI4,   eDouble,
     eString,  eBase64, eDate,
     eValueParserLast
   };

/** Helper class to represent the data of the current parsing step.
  */
  class ULXR_API_DECL0 ValueState : public XmlParserBase::ParserState
  {
   public:

   /** Constructs an ValueState.
     * @param  st  the actual state
     */
     ValueState (unsigned st);

   /** Transfers a Value into the ValueState.
     * @param  val   the value
     * @param  candel: @li true:  value is unique here, delete at end
     *                 @li false: value is shared here, delete it somewhere else
     */
     virtual void takeValue(Value *val, bool candel = true);

     virtual bool canDelete() const;

   /** Transfers a member name into the ValueState.
     * Used only for Structs.
     * @param  name   the member name
     */
     virtual void takeName(const CppString &name);

   /** Gets the name of the state.
     * Useful only for debugging.
     * @return the name of actual state
     */
     virtual CppString getStateName() const;

   /** Gets the name of the member.
     * Used only with Structs
     * @return the member name
     */
     CppString getName() const;

   /** Sets the name of the member.
     * Used only with Structs
     * @param  name   the member name
     */
     void setName(const CppString &name);

   /** Gets the Value of this state.
     * @return the Value.
     */
     Value* getValue() const;

   protected:

     Value     *value;
     bool       candel;

   private:

     CppString  name;

     ValueState(const ValueState&); // forbid this
     ValueState& operator= (const ValueState&);
  };

 protected:

/** Helper class to represent the data of the current parsing step
  * when the xml element is a Struct.
  */
  class ULXR_API_DECL0 MemberState : public ValueState
  {
   public:

   /** Constructs a MemberState.
     * @param  st   the actual state
     * @param  val  pointer to the resulting value
     */
     MemberState(unsigned st, Value *val);

   /** Transfers a Value into the ValueState.
     * @param  val   the value
     * @param  candel: @li true:  value is unique here, delete at end
     *                 @li false: value is shared here, delete it somewhere else
     */
     virtual void takeValue(Value *val, bool candel = true);

   /** Transfers a member name into the MemberState.
     * @param  name   the member name
     */
     virtual void takeName(const CppString &name);

   protected:

     Value  *mem_val;

   private:
     MemberState(const MemberState&); // forbid this
     MemberState& operator= (const MemberState&);
  };


/** Helper class to represent the data of the current parsing step
  * when the xml element is an Array.
  */
  class ULXR_API_DECL0 ArrayState : public ValueState
  {
   public:

   /** Constructs an ArrayState.
     * @param  st   the actual state
     */
     ArrayState(unsigned st);

   /** Transfers a Value into the ValueState.
     * @param  val   the value
     * @param  candel: @li true:  value is unique here, delete at end
     *                 @li false: value is shared here, delete it somewhere else
     */
     virtual void takeValue(Value *val, bool candel = true);

   private:
     ArrayState(const ArrayState&); // forbid this
     ArrayState& operator= (const ArrayState&);
  };

   friend class ValueState;

 protected:

 /** Gets a pointer to the topmost ValueState.
   * @return pointer to ValueState
   */
   virtual ValueState *getTopValueState() const = 0;
};


}  // namespace ulxr


#endif // ULXR_VALUEPARSE_BASE_H
