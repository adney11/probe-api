// Created by Sergey Kolomenkin

#include "stdafx.h"

#ifndef JSON_AMALGATED_H_INCLUDED
#define JSON_AMALGATED_H_INCLUDED
/// If defined, indicates that the source file is amalgated
/// to prevent private header inclusion.
#define JSON_IS_AMALGAMATION
#include "version.h"
#include "config.h"
#include "forwards.h"
#include "features.h"
#include "value.h"
#include "reader.h"
#include "writer.h"
#include "assertions.h"
#endif //ifndef JSON_AMALGATED_H_INCLUDED

// MSVS fix:
#undef min
#undef max

#include "json_tool.h"
#include "json_valueiterator.inl"

//------------------------------------------------------

#include "json_value.cpp"
#include "json_reader.cpp"
#include "json_writer.cpp"

//------------------------------------------------------
