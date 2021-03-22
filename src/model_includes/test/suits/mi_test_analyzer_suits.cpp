#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include "model_includes/test/fixtures/wrappers/mi_test_file_wrapper.hpp"
#include "model_includes/test/fixtures/wrappers/mi_test_include_wrapper.hpp"
#include "model_includes/test/fixtures/wrappers/mi_test_model_wrapper.hpp"

#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"
#include "model_includes/api/mi_model.hpp"

#include "test_tools/test_macros.hpp"

#include <std_fs>

/*------------------------------------------------------------------------------

TEST PLAN:

1. File locations
	1.1 Current directory
	1.2 Include directory
	1.3 File with std library name in project
	1.4 Include in local file
2. Include type
	2.1 User
	2.2 System
	2.3 Mix
3. Include status
	3.1 Resolved
	3.2 Unresolved
	3.3 Mix
	3.4 Std lib
4. Ignored folders
	4.1 Regular project
	4.2 CMake project
5. Ignore system includes
6. Ignore files
	6.1 Ignore destination
	6.2 Ignore source
7. File extensions
	7.1 Only C++ files
	7.2 Analyze without extension

------------------------------------------------------------------------------*/

namespace model_includes::test {

//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( AnalyzerTests, ModelIncludesFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_1_file_in_current_directory )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string classACppName{ "class_a.cpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classAHppPath{ projectDir + classAHppName };
	const std::string classACppPath{ projectDir + classACppName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );

	addFileToProject( mainCppName, "#include \"" + classAHppName + "\"" );

	addFileToProject( classAHppName, "class A{ A();  };" );
	addFileToProject(
		classACppName,
		"#include \"" + classAHppName + "\"		\n"
		"A::A();						\n"
	);

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		TEST_REQUIRE( classAHeader.isAvailable() );
		{
			TEST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			TEST_REQUIRE( classAHeader.checkIncludedByCount( 2 ) );

			IncludeWrapper includedByMain = classAHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includedByClassA = classAHeader.getIncludedBy( classACppPath );
			TEST_CHECK( includedByClassA.checkSource( classACppPath ) );
			TEST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		TEST_REQUIRE( classACpp.isAvailable() );
		{
			TEST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			TEST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			TEST_CHECK( includeClassAToHeader.checkDestination( classAHppPath ) );
			TEST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			TEST_CHECK( includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t1_2_file_in_include_directory )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string includeDir{ "/lib/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string classACppName{ "class_a.cpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classAHppPath{ includeDir + classAHppName };
	const std::string classACppPath{ includeDir + classACppName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addIncludePath( includeDir );

	addFileToProject( mainCppName, "#include \"" + classAHppName + "\"" );

	addFile( classAHppPath, "class A{ A();  };" );
	addFile(
		classACppPath,
		"#include \"" + classAHppName + "\"		\n"
		"A::A();						\n"
	);

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		TEST_REQUIRE( classAHeader.isAvailable() );
		{
			TEST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			TEST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = classAHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t1_3_file_with_std_library_name_in_project )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classLimitHppName{ "limits.h" };
	const std::string classLimitCppName{ "limits.cpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classAHppPath{ projectDir + classLimitHppName };
	const std::string classACppPath{ projectDir + classLimitCppName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );

	addFileToProject( mainCppName, "#include \"" + classLimitHppName + "\"" );

	addFileToProject( classLimitHppName, "class A{ A();  };" );
	addFileToProject(
		classLimitCppName,
		"#include \"" + classLimitHppName + "\"		\n"
		"A::A();						\n"
	);

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		TEST_REQUIRE( classAHeader.isAvailable() );
		{
			TEST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			TEST_REQUIRE( classAHeader.checkIncludedByCount( 2 ) );

			IncludeWrapper includedByMain = classAHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includedByClassA = classAHeader.getIncludedBy( classACppPath );
			TEST_CHECK( includedByClassA.checkSource( classACppPath ) );
			TEST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		TEST_REQUIRE( classACpp.isAvailable() );
		{
			TEST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			TEST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			TEST_CHECK( includeClassAToHeader.checkDestination( classAHppPath ) );
			TEST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			TEST_CHECK( includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t1_4_include_in_local_folder )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string includeDir{ "./i/" };

	const std::string classIncludeHppName{ "class_i.hpp" };
	const std::string classIncludeCppName{ "class_i.cpp" };

	const std::string classIncludeHppPath{ projectDir + includeDir + classIncludeHppName };
	const std::string classIncludeCppPath{ projectDir + includeDir + classIncludeCppName };

	setProjectDir( projectDir );

	addFile( classIncludeHppPath, "class I{ I(); };" );
	addFile( classIncludeCppPath,
		"#include \"" + includeDir + classIncludeHppName + "\""
		"I::I(){}"
	);

	addIncludePath( "." );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper file = model.findFile( classIncludeHppPath );
		TEST_REQUIRE( file.isAvailable() );
		{
			TEST_REQUIRE( file.checkIncludesCount( 0 ) );

			TEST_REQUIRE( file.checkIncludedByCount( 1 ) );
			IncludeWrapper include = file.getIncludedBy( classIncludeCppPath );
			TEST_CHECK( include.checkSource( classIncludeCppPath ) );
			TEST_CHECK( include.checkDestination( classIncludeHppPath ) );
			TEST_CHECK( include.checkType( IncludeType::User ) );
			TEST_CHECK( include.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper file = model.findFile( classIncludeCppPath );
		TEST_REQUIRE( file.isAvailable() );
		{
			TEST_REQUIRE( file.checkIncludesCount( 1 ) );
			IncludeWrapper include = file.getInclude( 0 );
			TEST_CHECK( include.checkSource( classIncludeCppPath ) );
			TEST_CHECK( include.checkDestination( classIncludeHppPath ) );
			TEST_CHECK( include.checkType( IncludeType::User ) );
			TEST_CHECK( include.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( file.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t2_1_include_type_user )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classAHppPath{ projectDir + classAHppName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName, "#include \"" + classAHppName + "\"" );
	addFile( classAHppPath, "class A{ A(){}  };" );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		TEST_REQUIRE( classAHeader.isAvailable() );
		{
			TEST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			TEST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = classAHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t2_2_include_type_system )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string systemName{ "iostream" };
	const std::string mainCppName{ "main.cpp" };

	const std::string & systemPath{ systemName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName, "#include <" + systemName +">" );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToSystem = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToSystem.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToSystem.checkDestination( systemPath ) );
			TEST_CHECK( includeMainToSystem.checkType( IncludeType::System ) );
			TEST_CHECK( includeMainToSystem.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper systemFile = model.findFile( systemPath );
		TEST_REQUIRE( systemFile.isAvailable() );
		{
			TEST_REQUIRE( systemFile.checkIncludesCount( 0 ) );

			TEST_REQUIRE( systemFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = systemFile.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( systemPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::System ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t2_3_include_type_mix )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string systemName{ "iostream" };
	const std::string userHeaderName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string & systemPath{ systemName };
	const std::string userHeaderPath{ projectDir + userHeaderName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName,
		"#include <" + systemName +">				\n"
		"#include \"" + userHeaderName +"\"			\n"
	);

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 2 ) );

			IncludeWrapper includeMainToSystem = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToSystem.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToSystem.checkDestination( systemPath ) );
			TEST_CHECK( includeMainToSystem.checkType( IncludeType::System ) );
			TEST_CHECK( includeMainToSystem.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 1 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( userHeaderName ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Unresolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper systemFile = model.findFile( systemPath );
		TEST_REQUIRE( systemFile.isAvailable() );
		{
			TEST_REQUIRE( systemFile.checkIncludesCount( 0 ) );

			TEST_REQUIRE( systemFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = systemFile.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( systemPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::System ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper userFile = model.findFile( userHeaderName );
		TEST_REQUIRE( userFile.isAvailable() );
		{
			TEST_REQUIRE( userFile.checkIncludesCount( 0 ) );

			TEST_REQUIRE( userFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = userFile.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( userHeaderName ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Unresolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t3_1_include_status_resolved )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string userHeaderName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string userHeaderPath{ projectDir + userHeaderName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName, "#include \"" + userHeaderName +"\"" );
	addFileToProject( userHeaderName, "class A{ A(){} }'" );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( userHeaderPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper userFile = model.findFile( userHeaderPath );
		TEST_REQUIRE( userFile.isAvailable() );
		{
			TEST_REQUIRE( userFile.checkIncludesCount( 0 ) );
			TEST_REQUIRE( userFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = userFile.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( userHeaderPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t3_2_include_status_unresolved )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string userHeaderName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName, "#include \"" + userHeaderName +"\"" );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( userHeaderName ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Unresolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper userFile = model.findFile( userHeaderName );
		TEST_REQUIRE( userFile.isAvailable() );
		{
			TEST_REQUIRE( userFile.checkIncludesCount( 0 ) );
			TEST_REQUIRE( userFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = userFile.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( userHeaderName ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Unresolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t3_3_include_status_mix )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classAHeaderName{ "class_a.hpp" };
	const std::string classBHeaderName{ "class_b.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string mainCppPath{ projectDir + mainCppName };
	const std::string classAHeaderPath{ projectDir + classAHeaderName };
	const std::string classBHeaderPath{ projectDir + classBHeaderName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName,
		"#include \"" + classAHeaderName +"\"			\n"
		"#include \"" + classBHeaderName +"\"			\n"
	);

	addFileToProject( classAHeaderName, "class A{ A(){} };" );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 2 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( classAHeaderPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includeMainToClassB = mainCpp.getInclude( 1 );
			TEST_CHECK( includeMainToClassB.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassB.checkDestination( classBHeaderName ) );
			TEST_CHECK( includeMainToClassB.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassB.checkStatus( IncludeStatus::Unresolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHeaderPath );
		TEST_REQUIRE( classAHeader.isAvailable() );
		{
			TEST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );
			TEST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = classAHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classAHeaderPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classBHeader = model.findFile( classBHeaderName );
		TEST_REQUIRE( classBHeader.isAvailable() );
		{
			TEST_REQUIRE( classBHeader.checkIncludesCount( 0 ) );
			TEST_REQUIRE( classBHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = classBHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classBHeaderName ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Unresolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t3_4_include_status_std_lib )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string headerName{ "assert.h" };
	const std::string mainCppName{ "main.cpp" };

	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName, "#include <" + headerName + ">" );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( headerName ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::System ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper userFile = model.findFile( headerName );
		TEST_REQUIRE( userFile.isAvailable() );
		{
			TEST_REQUIRE( userFile.checkIncludesCount( 0 ) );
			TEST_REQUIRE( userFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = userFile.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( headerName ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::System ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t4_1_ignored_folders_regular_project )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string ignoredDir{ "./i/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classIgnoreHppName{ "class_i.hpp" };
	const std::string classIgnoreCppName{ "class_i.cpp" };

	const std::string classAHppPath{ projectDir + classAHppName };
	const std::string mainCppPath{   projectDir + mainCppName };
	const std::string classIgnoreHppPath{ projectDir + ignoredDir + classIgnoreHppName };
	const std::string classIgnoreCppPath{ projectDir + ignoredDir + classIgnoreCppName };

	setProjectDir( projectDir );
	addIgnoredDir( ignoredDir );

	addFileToProject( mainCppName, "#include \"" + classAHppName + "\"" );
	addFile( classAHppPath, "class A{ A(){}  };" );
	addFile( classIgnoreHppPath, "class I{ I(); };" );
	addFile( classIgnoreCppPath,
		"#include \"" + classIgnoreHppName + "\""
		"I::I(){}"
	);

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		TEST_REQUIRE( classAHeader.isAvailable() );
		{
			TEST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );
			TEST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = classAHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t4_2_ignored_folders_cmake_project )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string ignoredDir{ "./i/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classIgnoreHppName{ "class_i.hpp" };
	const std::string classIgnoreCppName{ "class_i.cpp" };

	const std::string classAHppPath{ projectDir + classAHppName };
	const std::string mainCppPath{   projectDir + mainCppName };
	const std::string classIgnoreHppPath{ projectDir + ignoredDir + classIgnoreHppName };
	const std::string classIgnoreCppPath{ projectDir + ignoredDir + classIgnoreCppName };

	setProjectDir( projectDir );
	addIgnoredDir( ignoredDir );

	addFileToProject( mainCppName, "#include \"" + classAHppName + "\"" );
	addFile( classAHppPath, "class A{ A(){}  };" );
	addFile( classIgnoreHppPath, "class I{ I(); };" );
	addFile( classIgnoreCppPath,
		"#include \"" + classIgnoreHppName + "\""
		"I::I(){}"
	);

	addFileToCmake( mainCppPath );
	addFileToCmake( classIgnoreCppPath );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		TEST_REQUIRE( classAHeader.isAvailable() );
		{
			TEST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );
			TEST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain = classAHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t5_ignore_system_includes )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string systemName{ "iostream" };
	const std::string classA{ "classA.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject(
		mainCppName,
		R"(
		#include <iostream>
		#include "classA.hpp"
		)"
	);
	setIgnoreSystemIncludes( true );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClass = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClass.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClass.checkDestination( classA ) );
			TEST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t6_1_ignore_files_ignore_destination )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject(
		"main.cpp",
		R"(
		#include <boost/test/unit_test.hpp>
		#include "lib/classA.hpp"
		)"
	);

	addFileFilter( "boost/.*" );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( "/test_project/main.cpp" );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClass = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			TEST_CHECK( includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			TEST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHpp = model.findFile( "lib/classA.hpp" );
		TEST_REQUIRE( classAHpp.isAvailable() );
		{
			TEST_REQUIRE( classAHpp.checkIncludedByCount( 1 ) );

			IncludeWrapper includeMainToClass = classAHpp.getIncludedBy( "/test_project/main.cpp" );
			TEST_CHECK( includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			TEST_CHECK( includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			TEST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			TEST_REQUIRE( classAHpp.checkIncludesCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t6_2_ignore_files_ignore_source )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject(
		"main.cpp",
		R"(
		#include <boost/test/unit_test.hpp>
		#include "lib/classA.hpp"
		)"
	);
	addFileToProject(
		"lib_impl/classA.cpp",
		R"(
		#include "lib/classA.hpp"
		)"
	);

	addFileFilter( "boost/.*" );
	addFileFilter( "lib_impl/.*" );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( "/test_project/main.cpp" );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClass = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			TEST_CHECK( includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			TEST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHpp = model.findFile( "lib/classA.hpp" );
		TEST_REQUIRE( classAHpp.isAvailable() );
		{
			TEST_REQUIRE( classAHpp.checkIncludedByCount( 1 ) );

			IncludeWrapper includeMainToClass = classAHpp.getIncludedBy( "/test_project/main.cpp" );
			TEST_CHECK( includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			TEST_CHECK( includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			TEST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			TEST_REQUIRE( classAHpp.checkIncludesCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t7_1_file_extensions_only_cpp_files )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string classACppName{ "class_a.cpp" };
	const std::string cppHeaderName{ "header.hxx" };
	const std::string notCppHeaderName{ "header.txt" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classAHppPath{ projectDir + classAHppName };
	const std::string classACppPath{ projectDir + classACppName };
	const std::string cppHeaderPath{ projectDir + cppHeaderName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );

	addFileToProject( mainCppName, "#include \"" + classAHppName + "\"" );
	addFileToProject( cppHeaderName, "#include \"" + classAHppName + "\"" );
	addFileToProject( notCppHeaderName, "#include \"" + classAHppName + "\"" );

	addFileToProject( classAHppName, "class A{ A();  };" );
	addFileToProject(
		classACppName,
		"#include \"" + classAHppName + "\"		\n"
		"A::A();						\n"
	);

	addCppExtensions( { "*.cpp", "*.hpp", "*.hxx" } );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 4 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		TEST_REQUIRE( classAHeader.isAvailable() );
		{
			TEST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			TEST_REQUIRE( classAHeader.checkIncludedByCount( 3 ) );

			IncludeWrapper includedByMain = classAHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includedByClassA = classAHeader.getIncludedBy( classACppPath );
			TEST_CHECK( includedByClassA.checkSource( classACppPath ) );
			TEST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		TEST_REQUIRE( classACpp.isAvailable() );
		{
			TEST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			TEST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			TEST_CHECK( includeClassAToHeader.checkDestination( classAHppPath ) );
			TEST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			TEST_CHECK( includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper cppheaderFile = model.findFile( cppHeaderPath );
		TEST_REQUIRE( cppheaderFile.isAvailable() );
		{
			TEST_REQUIRE( cppheaderFile.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = cppheaderFile.getInclude( 0 );
			TEST_CHECK( includeClassAToHeader.checkSource( cppHeaderPath ) );
			TEST_CHECK( includeClassAToHeader.checkDestination( classAHppPath ) );
			TEST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			TEST_CHECK( includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( cppheaderFile.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t7_2_file_extensions_analyze_without_extension )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string classACppName{ "class_a.cpp" };
	const std::string headerName{ "header" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classAHppPath{ projectDir + classAHppName };
	const std::string classACppPath{ projectDir + classACppName };
	const std::string headerPath{ projectDir + headerName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );

	addFileToProject( mainCppName, "#include \"" + classAHppName + "\"" );
	addFileToProject( headerName, "#include \"" + classAHppName + "\"" );

	addFileToProject( classAHppName, "class A{ A();  };" );
	addFileToProject(
		classACppName,
		"#include \"" + classAHppName + "\"		\n"
		"A::A();						\n"
	);

	addCppExtensions( { "*.cpp", "*.hpp" } );
	setAnalyzeWithoutExtension( true );

	// Run
	ModelWrapper model = analyze();

	// Check
	TEST_REQUIRE( model.isAvailable() );
	TEST_REQUIRE( model.checkFilesCount( 4 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		TEST_REQUIRE( mainCpp.isAvailable() );
		{
			TEST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			TEST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			TEST_CHECK( includeMainToClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		TEST_REQUIRE( classAHeader.isAvailable() );
		{
			TEST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			TEST_REQUIRE( classAHeader.checkIncludedByCount( 3 ) );

			IncludeWrapper includedByMain = classAHeader.getIncludedBy( mainCppPath );
			TEST_CHECK( includedByMain.checkSource( mainCppPath ) );
			TEST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByMain.checkType( IncludeType::User ) );
			TEST_CHECK( includedByMain.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includedByClassA = classAHeader.getIncludedBy( classACppPath );
			TEST_CHECK( includedByClassA.checkSource( classACppPath ) );
			TEST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			TEST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			TEST_CHECK( includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		TEST_REQUIRE( classACpp.isAvailable() );
		{
			TEST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			TEST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			TEST_CHECK( includeClassAToHeader.checkDestination( classAHppPath ) );
			TEST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			TEST_CHECK( includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper headerFile = model.findFile( headerPath );
		TEST_REQUIRE( headerFile.isAvailable() );
		{
			TEST_REQUIRE( headerFile.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = headerFile.getInclude( 0 );
			TEST_CHECK( includeClassAToHeader.checkSource( headerPath ) );
			TEST_CHECK( includeClassAToHeader.checkDestination( classAHppPath ) );
			TEST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			TEST_CHECK( includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			TEST_REQUIRE( headerFile.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
