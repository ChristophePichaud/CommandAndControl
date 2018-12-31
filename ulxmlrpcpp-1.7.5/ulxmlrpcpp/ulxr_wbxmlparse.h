/***************************************************************************
                   ulxr_wbxmlparse.h  -  parse wbxml files
                             -------------------
    begin                : Fri Jan 09 2004
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_wbxmlparse.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_WBXMLPARSE
#define ULXR_WBXMLPARSE

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <vector>

#include <ulxmlrpcpp/ulxr_xmlparse_base.h>


namespace ulxr {


/** Base class for binary XML parsing.
  * This is a very limitited implementation which only handles a subset
  * which is enough for ulxmlrpcpp.
  * @ingroup grp_ulxr_parser
  */
class ULXR_API_DECL0 WbXmlParser : public XmlParserBase
{
 protected:

 /** Destroys the parser.
   * The derived class is responsible for cleaning up stack<ParserState*>.
   */
   virtual ~WbXmlParser();

 public:

 /** Constructs a parser.
   */
   WbXmlParser();

  /** Parse a piece of xml data.
    * @param buffer   pointer start of next data chunk
    * @param len      len of this chunk
    * @param isFinal  true: last call to parser
    * @return error condition, 0 = ok
    */
    virtual int parse(const char* buffer, int len, int isFinal);

  /** Gets the code for the current error.
    * @return error code
    */
    virtual unsigned getErrorCode() const;

  /** Gets the description for an error code
    * @param code  error code
    * @return  pointer to description
    */
    virtual CppString getErrorString(unsigned code) const;

  /** Gets the line number in the xml data.
    * Because the binary data has nothing like a line number, the current number
    * of start tags is returned.
    * @return  number of start tags
    */
    virtual int getCurrentLineNumber() const;

  /** Maps error codes to xml-rpc error codes.
    * Compatibility function due to expat, always return input.
    * @param  xpatcode   error code
    * @return  the according xml-rpc error
    */
    virtual int mapToFaultCode(int xpatcode) const;

    enum WbXmlToken
    {
      wbxml_SWITCH_PAGE  = 0x00,   //!< switch code page followed by a byte
      wbxml_END          = 0x01,   //!< end of attribut list or element
      wbxml_ENTITY       = 0x02,   //!< followed by a multibyte UCS-4 entity
      wbxml_PI           = 0x43,   //!< Processing instruction
      wbxml_OPAQUE       = 0xC3,   //!< opaque data followed by multibyte-length and data

      wbxml_LITERAL      = 0x04,   //!< unknown empty tag or attribut name, followed by table index
      wbxml_LITERAL_C    = 0x44,   //!< unknown tag with content, followed by table index
      wbxml_LITERAL_A    = 0x84,   //!< unknown tag with attributes, followed by table index
      wbxml_LITERAL_AC   = 0xC4,   //!< unknown tag with content and attributes, followed by table index

      wbxml_EXT_I_0      = 0x40,   //!< inline string extension 0 followed by string + terminator
      wbxml_EXT_I_1      = 0x41,   //!< inline string extension 1 followed by string + terminator
      wbxml_EXT_I_2      = 0x42,   //!< inline string extension 2 followed by string + terminator

      wbxml_EXT_T_0      = 0x80,   //!< inline integer extension 0 followed by multibyte
      wbxml_EXT_T_1      = 0x81,   //!< inline integer extension 1 followed by multibyte
      wbxml_EXT_T_2      = 0x82,   //!< inline integer extension 2 followed by multibyte

      wbxml_EXT_0        = 0xC0,   //!< single byte extension 0
      wbxml_EXT_1        = 0xC1,   //!< single byte extension 1
      wbxml_EXT_2        = 0xC2,   //!< single byte extension 2

      wbxml_STR_I        = 0x03,   //!< followed by inline string + terminator
      wbxml_STR_T        = 0x83,   //!< string table index followed by multibyte

      wbxml_TAG_FIRST    = 0x05,   //!< first value for empty elements
      wbxml_TAG_LAST     = 0x3f,   //!< last value for empty elements
      wbxml_TAG_C_FIRST  = 0x45,   //!< first value for elements with content
      wbxml_TAG_C_LAST   = 0x7F,   //!< last value for elements with content
      wbxml_TAG_A_FIRST  = 0x85,   //!< first value for elements without content but with attributes
      wbxml_TAG_A_LAST   = 0xBF,   //!< last value for elements without content but with attributes
      wbxml_TAG_AC_FIRST = 0xC5,   //!< first value for elements with content and attributes
      wbxml_TAG_AC_LAST  = 0xFF    //!< last value for elements with content and attributes
    };

    static const char     wbxml_START_SEQ_STR[];
    static const unsigned wbxml_START_SEQ_LEN;

  protected:

    class Token;
    class Attribute;

    typedef std::vector<Attribute>  Attributes;

 /** Tests if the current opening tag is to be parsed by this
   * inheritance level or by the parent.
   * @param  token  current well known token
   * @param  attr   tag attributes
   * @return true: element has been handled
   */
   bool testStartElement(unsigned token, const Attributes &attr);

 /** C++ callback for an opening XML tag.
   * @param  token  current literal tag
   * @param  attr   tag attributes
   */
   virtual void startElement(unsigned token, const Attributes &attr) =  0;

 /** C++ callback for a closing XML tag.
   * @return true: element has been handled
   */
   bool testEndElement();

 /** C++ callback for a closing XML tag.
   */
   virtual void endElement() = 0;

 /** Parses the content of the current xml element.
   * @param  s   the current chunk of text
   */
   virtual void charData(const std::string &s);

private:

   void init();

   struct ULXR_API_DECL0 WbXmlState
   {
     WbXmlState(unsigned state);

     unsigned    state;
     std::string current_str;
   };

   std::stack<WbXmlState> wbxmlstates;

   unsigned    wb_version;
   unsigned    wb_pubId;
   unsigned    wb_charset;

   unsigned    lastErrorCode;
   unsigned    tag_count;
};


class ULXR_API_DECL0 WbXmlParser::Attribute
{
  public:

    Attribute(const CppString &name, const CppString &value);

    CppString getName() const;

    CppString getValue() const;

  private:

    CppString attName;
    CppString attValue;
};


#define ULXR_MB_INT_14bit(x) (unsigned char) ((x >> 7) & 0x7F | 0x80) \
                             ,(unsigned char) (x & 0x7F)


}  // namespace ulxr


#endif // ULXR_WBXMLPARSE
