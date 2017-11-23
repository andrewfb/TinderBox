/*
 Copyright (c) 2015, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions and
    the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
    the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <QDir>
#include <QList>
#include <QSharedPointer>

#include "GeneratorBase.h"
#include "CinderBlock.h"
#include "Template.h"
#include "ProjectTemplate.h"

// Gathers list of files which needs to be copied, removes redundancies, and performs copy
class Collector {
  public:
	Collector( const QString &cinderPath );
	
    void    add( const Template::Item *item, const GeneratorConditions &conditions, const QString &outputDir, bool overwriteExisting );
  
	void copyFileOrDir( QFileInfo src, QFileInfo dst, bool overwriteExisting, bool replaceContents = false, const QString &replacePrefix = "",
						bool windowsLineEndings = false );
	void copyFileOrDir( const GeneratorConditions &conditions, QFileInfo src, QFileInfo dst, bool overwriteExisting, bool replaceContents = false, const QString &replacePrefix = "",
						bool windowsLineEndings = false );

	void	print();
  protected:
	struct Entry {
		Entry( const Template::Item *item, const GeneratorConditions &initalConditions, const QString &outputDir, bool overwriteExisting );
		const Template::Item					*mItem;
		QString									mOutputDir;
		std::vector<GeneratorConditions>		mConditions;
	};
	
	Entry*		find( const Template::Item *item );
	
	QString					mCinderPath;
	std::vector<Entry>		mEntries;
};

class Instancer {
  public:
    Instancer( const ProjectTemplate &projectTmpl );

	QList<Template::File>				getFilesMatchingConditions( const GeneratorConditions &conditions ) const;
	QList<Template::File>				getResourcesMatchingConditions( const std::vector<GeneratorConditions> &conditions ) const;
	QList<Template::IncludePath>		getIncludePathsMatchingConditions( const GeneratorConditions &conditions ) const;
	QList<Template::LibraryPath>		getLibraryPathsMatchingConditions( const GeneratorConditions &conditions ) const;
	QList<Template::FrameworkPath>		getFrameworkPathsMatchingConditions( const GeneratorConditions &conditions ) const;
	QList<Template::StaticLibrary>		getStaticLibrariesMatchingConditions( const GeneratorConditions &conditions ) const;
	QList<Template::DynamicLibrary>		getDynamicLibrariesMatchingConditions( const GeneratorConditions &conditions ) const;
	QList<Template::BuildSetting>		getBuildSettingsMatchingConditions( const GeneratorConditions &conditions ) const;
	QList<Template::PreprocessorDefine> getPreprocessorDefinesMatchingConditions( const GeneratorConditions &conditions ) const;
	QList<Template::OutputExtension>	getOutputExtensionsMatchingConditions( const GeneratorConditions &conditions ) const;

	// takes ownership of childGen
	void			addGenerator( GeneratorBase *childGen );
	void			addCinderBlock( const CinderBlock &block ) { mCinderBlocks.push_back( CinderBlockRef( new CinderBlock( block ) ) ); }
	void			instantiate( bool setupGit );

	QString         getProjectName() const;
	void            setProjectName( const QString &projName );

	QString         getNamePrefix() const;
	void            setNamePrefix( const QString &namePrefix );

	QString         getBaseLocation() const;
	void            setBaseLocation( const QString &baseLocation );

	QDir			getOutputDir() const { return QDir( getBaseLocation() + "/" + getProjectName() ); }

	QString			getWinRelCinderPath( const QString &relativeTo ) const { return getRelCinderPath( relativeTo ).replace( "/", "\\" ); }
	QString         getMacRelCinderPath( const QString &relativeTo ) const;
	QString			getCinderAbsolutePath() const { return mAbsCinderPath; }
	void            setCinderAbsolutePath( const QString &absCinderPath );

	QString			createDirectory( QString relPath ) const;
	QString			getAbsolutePath( QString relPath ) const;

  private:
	template<Template::File::Type FILE_TYPE>
	QList<Template::File> getFileTypeMatchingConditions( const std::vector<GeneratorConditions> &copyConditions, bool getCopyOnly ) const;

	bool			prepareGenerate();
	void			writeResourcesHeader( const std::vector<GeneratorConditions> &conditions ) const;
	void			copyAssets( const std::vector<GeneratorConditions> &conditions, Collector *cloner ) const;
	void			copyBareFiles( const std::vector<GeneratorConditions> &conditions, Collector *cloner ) const;
	QString         getRelCinderPath( const QString &relativeTo ) const;
	bool			setupGitRepo( const QString &dirPath );
	bool			initialCommitToGitRepo( const QString &dirPath );

    ProjectTemplate					mProjectTmpl;
    QSharedPointer<ProjectTemplate>	mChildTemplate;
    
	QString         mProjectName;
	QString         mNamePrefix;
	QString         mBaseLocation;
	QString         mAbsCinderPath;

	QList<GeneratorBaseRef>		mGenerators;
	QList<CinderBlockRef>		mCinderBlocks;
};
