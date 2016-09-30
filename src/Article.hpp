/*
 * hermod-cms - An Hermod based Content Management System
 *
 * Copyright (c) 2016 Cowlab
 *
 * Hermod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 
 * version 3 as published by the Free Software Foundation. You
 * should have received a copy of the GNU Lesser General Public
 * License along with this program, see LICENSE file for more details.
 * This program is distributed WITHOUT ANY WARRANTY see README file.
 *
 * Authors: Saint-Genest Gwenael <gwen@hooligan0.net>
 */
#ifndef ARTICLE_HPP
#define ARTICLE_HPP
#include <string>
#include <odb/core.hxx>
#include <String.hpp>

namespace hermod { namespace Database {
	class DB;
} }

using namespace hermod;
typedef unsigned long u32;

/**
 * @class Article
 * @brief Handle articles contents. This class maps objects from/to database
 *
 */
#pragma db object table("article")
class Article
{
public:
	explicit Article(hermod::Database::DB *db = 0);
	String chapeau(void);
	String content(void);
	bool   findById (std::string id);
	bool   findLast (bool publish = true);
	u32    getId(void);
	bool   save(void);
	void   setDB      (hermod::Database::DB *db);
	void   setChapeau (hermod::String chapeau);
	void   setContent (hermod::String content);
	void   setTitle   (hermod::String title);
	String title(void);
private:
	#pragma db transient
	hermod::Database::DB *mDb;
private:
	friend class odb::access;
	#pragma db id column("id")
	unsigned long mId;
	#pragma db column("username")
	int  mOwner;
	#pragma db column("created") type("INTEGER")
	unsigned long mDateCreated;
	#pragma db column("publish")
	int           mPublished;
	#pragma db column("title")
	std::string   mTitle;
	#pragma db column("chapeau")
	std::string   mChapeau;
	#pragma db column("content")
	std::string   mContent;
};

#pragma db view query("SELECT nextval('article_id_seq')")
struct article_seq_value
{
	unsigned long long value;
};

#endif
