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
#include <ctime>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>
#include <Database/DB.hpp>
#include <Log.hpp>
#include "Article.hpp"
#include "Article-odb.hxx"

using namespace odb::core;
using namespace hermod;

/**
 * @brief Default constructor
 *
 * @param db Pointer to a database object connected to the target dbms
 */
Article::Article(hermod::Database::DB *db)
{
	mDb    = db;
	mId    = 0;
	mOwner = 0;
	mDateCreated = 0;
	mPublished   = 0;
}

/**
 * @brief Get the article chapeau
 *
 * @return String Text of the chapeau
 */
String Article::chapeau(void)
{
	return String(mChapeau);
}

/**
 * @brief Get the article content
 *
 * @return String Text body of the article
 */
String Article::content(void)
{
	return String(mContent);
}

/**
 * @brief Find an article, identified by his ID
 *
 * @param id The requested article ID
 * @return boolean True if an article with this ID is found
 */
bool Article::findById (std::string id)
{
	if (mDb == 0)
	{
		Log::debug() << "Article::findById() No DB" << Log::endl;
		return false;
	}
	
	odb::core::database *db = mDb->get();
	
	int articleId = atoi(id.c_str());

	typedef odb::query<Article>  q;
	typedef odb::result<Article> qr;
	transaction t (db->begin ());
	qr r (db->query<Article> (q::mId == articleId));
	
	if (r.size() > 0)
	{
		qr::iterator i (r.begin());
		mId      = i->getId();
		mTitle   = i->title().toStdStr();
		mChapeau = i->chapeau().toStdStr();
		mContent = i->content().toStdStr();
		std::cerr << "Article: title length=" << String::number( mTitle.length() ) << std::endl;
		return true;
	}
	return false;
}

/**
 * @brief Find the last saved article
 *
 * @param publish When true find the last -published- article
 * @return boolean True if an article is found
 */
bool Article::findLast(bool publish)
{
	if (mDb == 0)
	{
		Log::debug() << "Article::findLast() No DB" << Log::endl;
		return false;
	}

	int sel = (publish == true) ? 1 : 0;

	odb::core::database *db = mDb->get();
	typedef odb::query<Article>  query;
	typedef odb::result<Article> qr;
	transaction t (db->begin ());
	query q ((query::mPublished == sel) + 
	         "ORDER BY" + query::mId + "DESC" +
	         "LIMIT" + query::_val(1) );
	qr r (db->query<Article>(q));

	if (r.size() > 0)
	{
		qr::iterator i (r.begin());
		mId      = i->getId();
		mTitle   = i->title().toStdStr();
		mChapeau = i->chapeau().toStdStr();
		mContent = i->content().toStdStr();
	}
}

/**
 * @brief Get the ID of this article
 *
 * @return unsigned long The current article ID
 */
unsigned long Article::getId(void)
{
	return mId;
}

/**
 * @brief Save the article to database
 *
 * @return boolean Result of the saving request (true on success)
 */
bool Article::save(void)
{
	if (mDb == 0)
	{
		Log::debug() << "Article::save() No DB" << Log::endl;
		return false;
	}
	
	std::time_t now = std::time(0);
	mDateCreated = now;
	mPublished = 1;
	try {
		odb::core::database *db = mDb->get();
		
		if (mId == 0)
		{
			// Use sequence to get the next available article ID
			transaction tseq (db->begin ());
			typedef odb::result<article_seq_value> rseq;
			rseq r (db->query<article_seq_value> ());
			rseq::iterator i(r.begin ());
			const article_seq_value& en (*i);
			mId = en.value;
			tseq.commit ();
		}
		
		transaction t (db->begin ());
		db->persist(this);
		t.commit ();
	} catch (const odb::exception& e) {
		Log::debug() << "Article::save Error: " << e.what() << Log::endl;
		return false;
	}
	return true;
}

/**
 * @brief Set a new value for the chapeau
 *
 * @param chapeau New text for the chapeau
 */
void Article::setChapeau(hermod::String chapeau)
{
	mChapeau = chapeau.toStdStr();
}

/**
 * @brief Set a new value for the content
 *
 * @param content New text for article body
 */
void Article::setContent(hermod::String content)
{
	mContent = content.toStdStr();
}

/**
 * @brief Set the DB object to use to access articles
 *
 * @param DB* Pointer to the database object
 */
void Article::setDB(hermod::Database::DB *db)
{
	mDb = db;
}

/**
 * @brief Set a new title for the article
 *
 * @param title New text fot the title
 */
void Article::setTitle(hermod::String title)
{
	mTitle = title.toStdStr();
}

/**
 * @brief Get the article title
 *
 * @return String The current title
 */
String Article::title(void)
{
	return String(mTitle);
}
/* EOF */
