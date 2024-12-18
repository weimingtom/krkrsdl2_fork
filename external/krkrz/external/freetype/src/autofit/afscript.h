/***************************************************************************/
/*                                                                         */
/*  afscript.h                                                             */
/*                                                                         */
/*    Auto-fitter scripts (specification only).                            */
/*                                                                         */
/*  Copyright 2013-2016 by                                                 */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


  /* The following part can be included multiple times. */
  /* Define `SCRIPT' as needed.                         */


  /* Add new scripts here.  The first and second arguments are the    */
  /* script name in lowercase and uppercase, respectively, followed   */
  /* by a description string.  Then comes the corresponding HarfBuzz  */
  /* script name tag, followed by a string of standard characters (to */
  /* derive the standard width and height of stems).                  */
  /*                                                                  */
  /* Note that fallback scripts only have a default style, thus we    */
  /* use `HB_SCRIPT_INVALID' as the HarfBuzz script name tag for      */
  /* them.                                                            */

  SCRIPT( arab, ARAB,
          "Arabic",
          HB_SCRIPT_ARABIC,
          HINTING_BOTTOM_TO_TOP,
          "\xD9\x84 \xD8\xAD \xD9\x80" )

  /* there are no simple forms for letters; we thus use two digit shapes */
  SCRIPT( beng, BENG,
          "Bengali",
          HB_SCRIPT_BENGALI,
          HINTING_TOP_TO_BOTTOM,
          "\xE0\xA7\xA6 \xE0\xA7\xAA" )

  SCRIPT( cyrl, CYRL,
          "Cyrillic",
          HB_SCRIPT_CYRILLIC,
          HINTING_BOTTOM_TO_TOP,
          "\xD0\xBE \xD0\x9E" )

  SCRIPT( deva, DEVA,
          "Devanagari",
          HB_SCRIPT_DEVANAGARI,
          HINTING_TOP_TO_BOTTOM,
          "\xE0\xA4\xA0 \xE0\xA4\xB5 \xE0\xA4\x9F" )

  SCRIPT( grek, GREK,
          "Greek",
          HB_SCRIPT_GREEK,
          HINTING_BOTTOM_TO_TOP,
          "\xCE\xBF \xCE\x9F" )

  SCRIPT( hebr, HEBR,
          "Hebrew",
          HB_SCRIPT_HEBREW,
          HINTING_BOTTOM_TO_TOP,
          "\xD7\x9D" ) /* ם */

  SCRIPT( knda, KNDA,
          "Kannada",
          HB_SCRIPT_KANNADA,
          HINTING_BOTTOM_TO_TOP,
          "\xE0\xB3\xA6 \xE0\xB2\xAC" )

  /* only digit zero has a simple shape in the Khmer script */
  SCRIPT( khmr, KHMR,
          "Khmer",
          HB_SCRIPT_KHMER,
          HINTING_BOTTOM_TO_TOP,
          "\xE1\x9F\xA0" )

  SCRIPT( khms, KHMS,
          "Khmer Symbols",
          HB_SCRIPT_INVALID,
          HINTING_BOTTOM_TO_TOP,
          "\xE1\xA7\xA1 \xE1\xA7\xAA" )

  /* only digit zero has a simple shape in the Lao script */
  SCRIPT( lao, LAO,
          "Lao",
          HB_SCRIPT_LAO,
          HINTING_BOTTOM_TO_TOP,
          "\xE0\xBB\x90" )

  SCRIPT( latn, LATN,
          "Latin",
          HB_SCRIPT_LATIN,
          HINTING_BOTTOM_TO_TOP,
          "o O 0" )

  SCRIPT( latb, LATB,
          "Latin Subscript Fallback",
          HB_SCRIPT_INVALID,
          HINTING_BOTTOM_TO_TOP,
          "\xE2\x82\x92 \xE2\x82\x80" )

  SCRIPT( latp, LATP,
          "Latin Superscript Fallback",
          HB_SCRIPT_INVALID,
          HINTING_BOTTOM_TO_TOP,
          "\xE1\xB5\x92 \xE1\xB4\xBC \xE2\x81\xB0" )

  SCRIPT( mymr, MYMR,
          "Myanmar",
          HB_SCRIPT_MYANMAR,
          HINTING_BOTTOM_TO_TOP,
          "\xE1\x80\x9D \xE1\x80\x84 \xE1\x80\x82" )

  SCRIPT( none, NONE,
          "no script",
          HB_SCRIPT_INVALID,
          HINTING_BOTTOM_TO_TOP,
          "" )

  /* there are no simple forms for letters; we thus use two digit shapes */
  SCRIPT( telu, TELU,
          "Telugu",
          HB_SCRIPT_TELUGU,
          HINTING_BOTTOM_TO_TOP,
          "\xE0\xB1\xA6 \xE0\xB1\xA7" )

  SCRIPT( thai, THAI,
          "Thai",
          HB_SCRIPT_THAI,
          HINTING_BOTTOM_TO_TOP,
          "\xE0\xB8\xB2 \xE0\xB9\x85 \xE0\xB9\x90" )

#ifdef AF_CONFIG_OPTION_INDIC

  SCRIPT( gujr, GUJR,
          "Gujarati",
          HB_SCRIPT_GUJARATI,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

  SCRIPT( guru, GURU,
          "Gurmukhi",
          HB_SCRIPT_GURMUKHI,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

  SCRIPT( limb, LIMB,
          "Limbu",
          HB_SCRIPT_LIMBU,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

  SCRIPT( mlym, MLYM,
          "Malayalam",
          HB_SCRIPT_MALAYALAM,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

  SCRIPT( orya, ORYA,
          "Oriya",
          HB_SCRIPT_ORIYA,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

  SCRIPT( sinh, SINH,
          "Sinhala",
          HB_SCRIPT_SINHALA,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

  SCRIPT( sund, SUND,
          "Sundanese",
          HB_SCRIPT_SUNDANESE,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

  SCRIPT( sylo, SYLO,
          "Syloti Nagri",
          HB_SCRIPT_SYLOTI_NAGRI,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

  SCRIPT( taml, TAML,
          "Tamil",
          HB_SCRIPT_TAMIL,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

  SCRIPT( tibt, TIBT,
          "Tibetan",
          HB_SCRIPT_TIBETAN,
          HINTING_BOTTOM_TO_TOP,
          "o" ) /* XXX */

#endif /* AF_CONFIG_OPTION_INDIC */

#ifdef AF_CONFIG_OPTION_CJK

  SCRIPT( hani, HANI,
          "CJKV ideographs",
          HB_SCRIPT_HAN,
          HINTING_BOTTOM_TO_TOP,
          "\xE7\x94\xB0 \xE5\x9B\x97" )

#endif /* AF_CONFIG_OPTION_CJK */


/* END */
