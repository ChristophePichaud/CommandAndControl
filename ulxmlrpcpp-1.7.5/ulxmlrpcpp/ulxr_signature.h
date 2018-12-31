/***************************************************************************
         ulxr_signature.h  -  handle method signatures
                             -------------------
    begin                : Sat Mar 23 2002
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_signature.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_SIGNATURE_H
#define ULXR_SIGNATURE_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header


namespace ulxr {


class Value;
class ValueBase;
class Void;


/** Convenience class to generate larger signature strings for RPC methods:
  *  Usage examples:
  * <pre>
  *    Signature()
  *         .addParam(Integer())
  *         .addParam(String())
  *         .....
  *
  *    Signature(Integer()).addParam(String())
  *         .....
  *    Signature() << RpcString() << Integer();
  * </pre>
  * @ingroup grp_ulxr_rpc
  */
class ULXR_API_DECL0 Signature
{
  public:

  /** Constructs an empty signature
    */
    Signature();

  /** Constructs a signature from a single parameter
    * @param  s  rpc name of the parameter (int, string, ...)
    */
    Signature(const CppString &s);

  /** Constructs a signature from a single parameter
    * @param  v  rpc value
    */
    Signature(const ValueBase &v);

  /** Constructs a signature from a void type.
    * Only exists for completeness and yields the same as the default constructor.
    * @param  v  void value
    */
    Signature(const Void &v);

  /** Adds another parameter to the signature
    * @param  s  rpc name of the parameter
    */
    Signature& addParam(const CppString &s);

  /** Adds another parameter to the signature
    * @param  v  rpc value
    */
    Signature& addParam(const Value &v);

  /** Adds another parameter to the signature
    * @param  s  rpc name of the parameter
    */
    Signature& operator<<(const CppString &s);

  /** Adds another parameter to the signature
    * @param  v  rpc value
    */
    Signature& operator<<(const Value &v);

  /** Return the complete signature string
    * @return signature
    */
    CppString getString() const;

  private:

    CppString     sig;
};


}  // namespace ulxr


#endif // ULXR_SIGNATURE_H

