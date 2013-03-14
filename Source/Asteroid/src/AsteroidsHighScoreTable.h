// *****************************************************************************
//  AsteroidHighScoreTable   version:  1.0   Ankur Sheel  date: 2013/03/14
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#ifndef AsteroidsHighScoreTable_h__
#define AsteroidsHighScoreTable_h__

#include "HighScoreTable.h"

class cAsteroidsHighScoreTable :
	public GameBase::cHighScoreTable
{
public:
	cAsteroidsHighScoreTable();
	~cAsteroidsHighScoreTable();
};

#endif