/*
 * Copyright (c) 2000-2012, 2013 S.Bhatnagar
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/* $Id$ */
// #if !defined(CLBOOL_H)
// #define CLBOOL_H
//
// All entries must be in all-uppercase.
// Number of True and False entries can be different.
//
#include <string>
#include <vector>
vector<string> clBoolTrue, clBoolFalse;
clBoolTrue.push_back("1");       clBoolFalse.push_back("0");        // Computer'ees
clBoolTrue.push_back("TRUE");    clBoolFalse.push_back("FALSE");    // Logic'ees
clBoolTrue.push_back("T");       clBoolFalse.push_back("F");        // Lazy Logic'ess ;-)
clBoolTrue.push_back("HAAN");    clBoolFalse.push_back("NAHEE");    // Hindi
clBoolTrue.push_back("JEEHAAN"); clBoolFalse.push_back("JEENAHEE"); // Hindi/Punjabe
clBoolTrue.push_back("AWANU");   clBoolFalse.push_back("KAADHU");   // Telugu
clBoolTrue.push_back("HUM");     clBoolFalse.push_back("UMM");      // Thoughtful
clBoolTrue.push_back("POSITIVE");clBoolFalse.push_back("NEGATIVE"); // Military'ees
clBoolTrue.push_back("+IV");     clBoolFalse.push_back("-IV");      // Physicist'ees
clBoolTrue.push_back("YES");     clBoolFalse.push_back("NO");       // English
clBoolTrue.push_back("AYE");     clBoolFalse.push_back("NAY");      // Old English (now American?)
clBoolTrue.push_back("SI");      clBoolFalse.push_back("NO");       // Spanish
clBoolTrue.push_back("JA");      clBoolFalse.push_back("NEE");      // Dutch
clBoolTrue.push_back("YA");      clBoolFalse.push_back("NA");       // (American) Slan
clBoolTrue.push_back("OUI");     clBoolFalse.push_back("NON");      // For Mauritian Croele (not French! :))
clBoolTrue.push_back(":-)");     clBoolFalse.push_back(":-(");      // SMS'ees
clBoolTrue.push_back(":-)");     clBoolFalse.push_back(":-(");      // SMS'ees
clBoolTrue.push_back("Y");       clBoolFalse.push_back("N");        // Lazy English ;-)


// #endif
