/**
 * Author:
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-08-26
 * Modified:
 * license:
 * Description: .
 * File:				gdsCpp.hpp
 */

// ############################ DETAILED DESCRIPTION ################################
// To understand format better: http://boolean.klaasholwerda.nl/interface/bnf/gdsformat.html
// =============================== GENERAL RECORDS ==================================
/*                                      <Record>     < Done? >
*                                       [HEADER]        [X]
*                                       [BGNLIB]        [X]
*                                       [LIBNAME]       [X]
*                                       [REFLIBS]       [ ](Don't bother here unless needed.)
*                                       [FONTS]         [ ](Don't bother here unless needed.)
*                                       [ATTRTABLE]     [ ](Don't bother here unless needed.)
*                                       [GENERATIONS]   [X]
*                                       [<FormatType>]  [ ](Don't bother here unless needed.)
*                                       [UNITS]         [X]
*                                       [BGNSTR]        [ ]
*                                       [STRNAME]       [X]
*                                       [ENDLIB]        [ ]
*/
// =============================== ELEMENT RECORDS ==================================
// Element records form part of a structure
// --------------------------------- 1: BOUNDARY        [X]--------------------------
//              PLEX       //Ignore for now - seems to only be used with nodes
//              LAYER
//              DATATYPE
//              XY
//              PROPATTR
//              PROPVALUE
// ----------------------------------- 2: PATH          [X]--------------------------
//              PLEX       //Ignore for now - seems to only be used with nodes
//              LAYER
//              DATATYPE
//              PATHTYPE
//              WIDTH
//              XY
//              PROPATTR
//              PROPVALUE
// --------------------------- 3: STRUCTURE REFERENCE   [X]--------------------------
//              SNAME
//              STRANS Transformation )
//              MAG                   )
//              ANGLE                 )
//              XY
//              PROPATTR
//              PROPVALUE
// ----------------------------- 4: ARRAY REFERENCE     [X]--------------------------
//              PLEX       //Ignore for now - seems to only be used with nodes
//              SNAME
//              STRANS Transformation )
//              MAG                   )
//              ANGLE                 )
//              COLROW
//              XY
//              PROPATTR
//              PROPVALUE
// ----------------------------------- 5: TEXT          [X]--------------------------
//              PLEX       //Ignore for now - seems to only be used with nodes
//              LAYER
//              TEXTTYPE
//              PRESENTATION
//              PATHTYPE
//              WIDTH
//              STRANS Transformation )
//              MAG                   )
//              ANGLE                 )
//              XY
//              STRING
//              PROPATTR
//              PROPVALUE
// ----------------------------------- 6: NODE          [X]--------------------------
//              PLEX
//              LAYER
//              NODETYPE
//              XY
//              PROPATTR
//              PROPVALUE
// ----------------------------------- 7: BOX           [X]--------------------------
//              PLEX       //Ignore for now - seems to only be used with nodes
//              LAYER
//              BOXTYPE
//              XY
//              PROPATTR
//              PROPVALUE

#ifndef gdsCpp
#define gdsCpp

using namespace std;

// ==================== Class declarations =====================
class gdscpp;                                  // All encompassing master class
class gdsSTR;                                  // Subclass containing all structures
class gdsBOUNDARY;                             // x2subclass belonging to gdsSTR
class gdsPATH;                                 // ''
class gdsSREF;                                 // ''
class gdsAREF;                                 // ''
class gdsTEXT;                                 // ''
class gdsNODE;                                 // ''
class gdsBOX;                                  // ''

// ========================== Includes ========================
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <map>
#include <fstream>
#include <bitset>
#include <algorithm>
#include "gdsParser.hpp"
#include "gdsForge.hpp"

// ===================== Class Definitions ====================
/*
This is the highest class, holding all the information of a single .gds file.
*/
class gdscpp{  // (GDS file)
  private:
    int version_number=7;                     // GDS version number. Default to 7
    int generations = 3;                      // Default generations. Don't really use
    double units[2] = {0.001, 1e-9};          // micron default
    vector<gdsSTR> STR;                       // Holds all the structures of the gds file
    vector<int> last_modified;                // TODO: Default to current datetime if unread
    string library_name = "Untitled_library"; // Default libname

    int GDSrecord2ASCII(char *recIn);         // Does it belong here???
  public:
    gdscpp(){};
    ~gdscpp(){};
    void setSTR(gdsSTR target_structure);
    void setSTR(vector<gdsSTR> target_structure);
    //void setSTR(gdsSTR exVec){STR.push_back(exVec);};
    void getSTR(vector<gdsSTR>& exVec){exVec = STR;};
    int import(string fileName);
    int write(string fileName);
    int quick2ASCII(string fileName);         // does not store data, legacy code.
    double get_database_units();
    void to_str();
    void reset();
};

/*
Subclass of gdscpp which holds a structure's information.
*/
class gdsSTR{
  private:

  public:
    gdsSTR(){};
    ~gdsSTR(){};

    void to_str();
    void reset();

    string name = "\0";

    vector<int> last_modified;
    vector<gdsBOUNDARY> BOUNDARY;
    vector<gdsPATH> PATH;
    vector<gdsSREF> SREF;
    vector<gdsAREF> AREF;
    vector<gdsTEXT> TEXT;
    vector<gdsNODE> NODE;
    vector<gdsBOX> BOX;

};

/*
Subclass of a gdsSTR class which stores boundary structures.
*/
class gdsBOUNDARY{
  private:

  public:
    gdsBOUNDARY(){};
    ~gdsBOUNDARY(){};

    void to_str();
    void reset();

    unsigned int layer = 0;
    unsigned int dataType = 0;
    vector<int> xCor;
    vector<int> yCor;
    // Property attribute seldom used. Valid range specified from 1 to  127
    // If value is 0, no attribute was set
    unsigned int propattr = 0;
    string propvalue = "\0";
};

/*
Subclass of a gdsSTR class which stores path structures.

Pathtype: This record contains a value that describes the type of path endpoints. The value is
0 for square-ended paths that endflush with their endpoints
1 for round-ended paths
2 for square-ended paths that extend a half-width beyond their endpoints
*/
class gdsPATH{
  private:

  public:
    gdsPATH(){};
    ~gdsPATH(){};

    void to_str();
    void reset();

    unsigned int layer = 0;
    unsigned int dataType = 0;
    unsigned int pathtype = 0;
    unsigned int width = 0;
    vector<int> xCor;
    vector<int> yCor;
    unsigned int propattr = 0;
    string propvalue = "\0";
};

/*
Subclass of a gdsSTR class which stores structure reference structures.
*/
class gdsSREF{
  private:

  public:
    gdsSREF(){};
    ~gdsSREF(){};

    void to_str();
    void reset();

    string name = "\0";
    bitset<16> sref_flags;
    bool reflection = false;  // boolean,    flag at bit 0
    double scale = 1;         // multiplier, flag at bit 13
    double angle = 0;         // degrees,    flag at bit 14
    int xCor = 0;
    int yCor = 0;
    unsigned int propattr = 0;
    string propvalue = "\0";
};

/*
Subclass of a gdsSTR class which stores array reference structures.
*/
class gdsAREF{
  private:

  public:
    gdsAREF(){};
    ~gdsAREF(){};

    void to_str();
    void reset();

                                  // PLEX
    string name = "\0";           // SNAME
    bitset<16> aref_transformation_flags;
    bool reflection = false;      // STRANS
    double angle = 0;             // subSTRANS
    double scale = 1;             // subSTRANS
    int colrow = 0;
    int xCor = 0;                 // XY
    int yCor = 0;                 // XY
    unsigned int propattr = 0;
    string propvalue = "\0";
};

/*
Subclass of a gdsSTR class which stores text structures.
*/
class gdsTEXT{
  private:

  public:
    gdsTEXT(){};
    ~gdsTEXT(){};

    void to_str();
    void reset();

    /* Bits 10 and 11, taken together as a binary number,
     * specify the font (00 means font 0, 01 rneans font 1,
     * 10 means font 2, and 11 means font 3). Bits 12 and 13 specify the vertical justification
     * (00 means top, 01 means middle, and 10 means bottom). Bits 14 and 15 specify the horizontal justification
     * (00 means left, 01 means center, and 10 means right). Bits 0 through 9 are reserved for future use and must be cleared.
     * If this record is omitted, then top-left justification and font 0 are assumed.*/
    bitset<16> presentation_flags;
    string textbody = "\0";
    unsigned int layer = 0;
    int text_type = 0;
    int path_type = 0;
    int width = 0;
    bitset<16> text_transformation_flags;
    double scale = 1;
    double angle = 0;
    int xCor = 0;
    int yCor = 0;
    unsigned int propattr = 0;
    string propvalue = "\0";
};

/*
Subclass of a gdsSTR class which stores node structures.
*/
class gdsNODE{
  private:

  public:
    gdsNODE(){};
    ~gdsNODE(){};

    void to_str();
    void reset();


    unsigned int plex = 0;  // optional
    unsigned int layer = 0;
    unsigned int nodetype = 0;
    // A maximum of 50 coordinates
    vector<int> xCor;
    vector<int> yCor;
    unsigned int propattr = 0;
    string propvalue = "\0";
};

/*
Subclass of a gdsSTR class which stores box structures.
*/
class gdsBOX{
  private:

  public:
    gdsBOX(){};
    ~gdsBOX(){};

    void to_str();
    void reset();

    unsigned int plex = 0;  // optional
    unsigned int layer = 0;
    unsigned int boxtype = 0;
    // A maximum of 50 coordinates
    vector<int> xCor;
    vector<int> yCor;
    unsigned int propattr = 0;
    string propvalue = "\0";
};

#endif