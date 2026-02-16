// File /Engine/Utils/Preprocessor.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <boost/preprocessor.hpp>

#define CONCAT_(a,b) a##b
#define CONCAT(a,b) CONCAT_(a,b)

#define UNIQUE_VAR(var) CONCAT(CONCAT(var,_),__LINE__)

#define VA_TO_SEQ(...) BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)
#define VA_TO_TUPLE(...) BOOST_PP_VARIADIC_TO_TUPLE(__VA_ARGS__)
#define SEQ_TO_TUPLE(seq) BOOST_PP_SEQ_TO_TUPLE(seq)
#define TUPLE_TO_SEQ(tuple) BOOST_PP_TUPLE_TO_SEQ(tuple)

#define SEQ_IS_EMPTY(seq) BOOST_PP_SEQ_DETAIL_IS_EMPTY(seq)
#define VA_IS_EMPTY(...) SEQ_IS_EMPTY(VA_TO_SEQ(__VA_ARGS__))

#define SEQ_IS_NOT_EMPTY(seq ) BOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq)
#define VA_IS_NOT_EMPTY(...) SEQ_IS_NOT_EMPTY(VA_TO_SEQ(__VA_ARGS__))

#define SEQ_CONDITION(notEmptyOp,emptyOp,seq) BOOST_PP_IIF(SEQ_IS_NOT_EMPTY(seq),notEmptyOp,emptyOp)
#define VA_CONDITION(notEmptyOp,emptyOp,...) SEQ_CONDITION(VA_TO_SEQ(__VA_ARGS__),notEmptyOp,emptyOp)

#define IF_SEQ(op,seq) SEQ_CONDITION(op,,seq)
#define IF_VA(op,...) IF_SEQ(op,VA_TO_SEQ(__VA_ARGS__))