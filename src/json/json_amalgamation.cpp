// This file is created by Sergey Kolomenkin

#include "stdafx.h"

#ifndef JSON_AMALGATED_H_INCLUDED
#define JSON_AMALGATED_H_INCLUDED
/// If defined, indicates that the source file is amalgated
/// to prevent private header inclusion.
#define JSON_IS_AMALGAMATION
#include <json/version.h>
#include <json/config.h>
#include <json/forwards.h>
#include <json/features.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/assertions.h>
#endif //ifndef JSON_AMALGATED_H_INCLUDED

// MSVS fix:
#undef min
#undef max

#include <json/json_tool.h>
#include <json/json_valueiterator.inl>

//------------------------------------------------------

#include <json/json_value.cpp>
#include <json/json_reader.cpp>
#include <json/json_writer.cpp>

//------------------------------------------------------
