#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include "model_includes/test/fixtures/wrappers/mi_test_file_wrapper.hpp"
#include "model_includes/test/fixtures/wrappers/mi_test_include_wrapper.hpp"
#include "model_includes/test/fixtures/wrappers/mi_test_model_wrapper.hpp"

#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"
#include "model_includes/api/mi_model.hpp"

#include <boost/test/unit_test.hpp>

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

namespace model_includes::test
{
//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
// NOLINTNEXTLINE(fuchsia-statically-constructed-objects,cert-err58-cpp)
BOOST_FIXTURE_TEST_SUITE( AnalyzerTests, ModelIncludesFixture )

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t1_1_file_in_current_directory )
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
		classACppName, "#include \"" + classAHppName +
						   "\"		\n"
						   "A::A();						\n" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 2 ) );

			IncludeWrapper includedByMain =
				classAHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includedByClassA =
				classAHeader.getIncludedBy( classACppPath );
			BOOST_CHECK( includedByClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		BOOST_REQUIRE( classACpp.isAvailable() );
		{
			BOOST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			BOOST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			BOOST_CHECK(
				includeClassAToHeader.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t1_2_file_in_include_directory )
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
		classACppPath, "#include \"" + classAHppName +
						   "\"		\n"
						   "A::A();						\n" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				classAHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t1_3_file_with_std_library_name_in_project )
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
		classLimitCppName, "#include \"" + classLimitHppName +
							   "\"		\n"
							   "A::A();						\n" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 2 ) );

			IncludeWrapper includedByMain =
				classAHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includedByClassA =
				classAHeader.getIncludedBy( classACppPath );
			BOOST_CHECK( includedByClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		BOOST_REQUIRE( classACpp.isAvailable() );
		{
			BOOST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			BOOST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			BOOST_CHECK(
				includeClassAToHeader.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t1_4_include_in_local_folder )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string includeDir{ "./i/" };

	const std::string classIncludeHppName{ "class_i.hpp" };
	const std::string classIncludeCppName{ "class_i.cpp" };

	const std::string classIncludeHppPath{
		projectDir + includeDir + classIncludeHppName };
	const std::string classIncludeCppPath{
		projectDir + includeDir + classIncludeCppName };

	setProjectDir( projectDir );

	addFile( classIncludeHppPath, "class I{ I(); };" );
	addFile(
		classIncludeCppPath, "#include \"" + includeDir + classIncludeHppName +
								 "\""
								 "I::I(){}" );

	addIncludePath( "." );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper file = model.findFile( classIncludeHppPath );
		BOOST_REQUIRE( file.isAvailable() );
		{
			BOOST_REQUIRE( file.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( file.checkIncludedByCount( 1 ) );
			IncludeWrapper include = file.getIncludedBy( classIncludeCppPath );
			BOOST_CHECK( include.checkSource( classIncludeCppPath ) );
			BOOST_CHECK( include.checkDestination( classIncludeHppPath ) );
			BOOST_CHECK( include.checkType( IncludeType::User ) );
			BOOST_CHECK( include.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper file = model.findFile( classIncludeCppPath );
		BOOST_REQUIRE( file.isAvailable() );
		{
			BOOST_REQUIRE( file.checkIncludesCount( 1 ) );
			IncludeWrapper include = file.getInclude( 0 );
			BOOST_CHECK( include.checkSource( classIncludeCppPath ) );
			BOOST_CHECK( include.checkDestination( classIncludeHppPath ) );
			BOOST_CHECK( include.checkType( IncludeType::User ) );
			BOOST_CHECK( include.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( file.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t2_1_include_type_user )
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
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				classAHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t2_2_include_type_system )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string systemName{ "iostream" };
	const std::string mainCppName{ "main.cpp" };

	const std::string & systemPath{ systemName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName, "#include <" + systemName + ">" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToSystem = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToSystem.checkSource( mainCppPath ) );
			BOOST_CHECK( includeMainToSystem.checkDestination( systemPath ) );
			BOOST_CHECK( includeMainToSystem.checkType( IncludeType::System ) );
			BOOST_CHECK(
				includeMainToSystem.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper systemFile = model.findFile( systemPath );
		BOOST_REQUIRE( systemFile.isAvailable() );
		{
			BOOST_REQUIRE( systemFile.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( systemFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				systemFile.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( systemPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::System ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t2_3_include_type_mix )
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
	addFileToProject(
		mainCppName, "#include <" + systemName +
						 ">				\n"
						 "#include \"" +
						 userHeaderName + "\"			\n" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 2 ) );

			IncludeWrapper includeMainToSystem = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToSystem.checkSource( mainCppPath ) );
			BOOST_CHECK( includeMainToSystem.checkDestination( systemPath ) );
			BOOST_CHECK( includeMainToSystem.checkType( IncludeType::System ) );
			BOOST_CHECK(
				includeMainToSystem.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 1 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( userHeaderName ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper systemFile = model.findFile( systemPath );
		BOOST_REQUIRE( systemFile.isAvailable() );
		{
			BOOST_REQUIRE( systemFile.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( systemFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				systemFile.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( systemPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::System ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper userFile = model.findFile( userHeaderName );
		BOOST_REQUIRE( userFile.isAvailable() );
		{
			BOOST_REQUIRE( userFile.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( userFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				userFile.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( userHeaderName ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Unresolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_1_include_status_resolved )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string userHeaderName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string userHeaderPath{ projectDir + userHeaderName };
	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName, "#include \"" + userHeaderName + "\"" );
	addFileToProject( userHeaderName, "class A{ A(){} }'" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( userHeaderPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper userFile = model.findFile( userHeaderPath );
		BOOST_REQUIRE( userFile.isAvailable() );
		{
			BOOST_REQUIRE( userFile.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( userFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				userFile.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( userHeaderPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_2_include_status_unresolved )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string userHeaderName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string mainCppPath{ projectDir + mainCppName };

	setProjectDir( projectDir );
	addFileToProject( mainCppName, "#include \"" + userHeaderName + "\"" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( userHeaderName ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper userFile = model.findFile( userHeaderName );
		BOOST_REQUIRE( userFile.isAvailable() );
		{
			BOOST_REQUIRE( userFile.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( userFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				userFile.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( userHeaderName ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Unresolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_3_include_status_mix )
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
	addFileToProject(
		mainCppName, "#include \"" + classAHeaderName +
						 "\"			\n"
						 "#include \"" +
						 classBHeaderName + "\"			\n" );

	addFileToProject( classAHeaderName, "class A{ A(){} };" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 2 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( classAHeaderPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includeMainToClassB = mainCpp.getInclude( 1 );
			BOOST_CHECK( includeMainToClassB.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassB.checkDestination( classBHeaderName ) );
			BOOST_CHECK( includeMainToClassB.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassB.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHeaderPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				classAHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classAHeaderPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classBHeader = model.findFile( classBHeaderName );
		BOOST_REQUIRE( classBHeader.isAvailable() );
		{
			BOOST_REQUIRE( classBHeader.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( classBHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				classBHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classBHeaderName ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Unresolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_4_include_status_std_lib )
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
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK( includeMainToClassA.checkDestination( headerName ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::System ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper userFile = model.findFile( headerName );
		BOOST_REQUIRE( userFile.isAvailable() );
		{
			BOOST_REQUIRE( userFile.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( userFile.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				userFile.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( headerName ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::System ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_1_ignored_folders_regular_project )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string ignoredDir{ "./i/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classIgnoreHppName{ "class_i.hpp" };
	const std::string classIgnoreCppName{ "class_i.cpp" };

	const std::string classAHppPath{ projectDir + classAHppName };
	const std::string mainCppPath{ projectDir + mainCppName };
	const std::string classIgnoreHppPath{
		projectDir + ignoredDir + classIgnoreHppName };
	const std::string classIgnoreCppPath{
		projectDir + ignoredDir + classIgnoreCppName };

	setProjectDir( projectDir );
	addIgnoredDir( ignoredDir );

	addFileToProject( mainCppName, "#include \"" + classAHppName + "\"" );
	addFile( classAHppPath, "class A{ A(){}  };" );
	addFile( classIgnoreHppPath, "class I{ I(); };" );
	addFile(
		classIgnoreCppPath, "#include \"" + classIgnoreHppName +
								"\""
								"I::I(){}" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				classAHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_2_ignored_folders_cmake_project )
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string ignoredDir{ "./i/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string mainCppName{ "main.cpp" };

	const std::string classIgnoreHppName{ "class_i.hpp" };
	const std::string classIgnoreCppName{ "class_i.cpp" };

	const std::string classAHppPath{ projectDir + classAHppName };
	const std::string mainCppPath{ projectDir + mainCppName };
	const std::string classIgnoreHppPath{
		projectDir + ignoredDir + classIgnoreHppName };
	const std::string classIgnoreCppPath{
		projectDir + ignoredDir + classIgnoreCppName };

	setProjectDir( projectDir );
	addIgnoredDir( ignoredDir );

	addFileToProject( mainCppName, "#include \"" + classAHppName + "\"" );
	addFile( classAHppPath, "class A{ A(){}  };" );
	addFile( classIgnoreHppPath, "class I{ I(); };" );
	addFile(
		classIgnoreCppPath, "#include \"" + classIgnoreHppName +
								"\""
								"I::I(){}" );

	addFileToCmake( mainCppPath );
	addFileToCmake( classIgnoreCppPath );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByMain =
				classAHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_ignore_system_includes )
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
		)" );
	setIgnoreSystemIncludes( true );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClass = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClass.checkSource( mainCppPath ) );
			BOOST_CHECK( includeMainToClass.checkDestination( classA ) );
			BOOST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t6_1_ignore_files_ignore_destination )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject(
		"main.cpp",
		R"(
		#include <boost/test/unit_test.hpp>
		#include "lib/classA.hpp"
		)" );

	addFileFilter( "boost/.*" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( "/test_project/main.cpp" );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClass = mainCpp.getInclude( 0 );
			BOOST_CHECK(
				includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			BOOST_CHECK(
				includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			BOOST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHpp = model.findFile( "lib/classA.hpp" );
		BOOST_REQUIRE( classAHpp.isAvailable() );
		{
			BOOST_REQUIRE( classAHpp.checkIncludedByCount( 1 ) );

			IncludeWrapper includeMainToClass =
				classAHpp.getIncludedBy( "/test_project/main.cpp" );
			BOOST_CHECK(
				includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			BOOST_CHECK(
				includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			BOOST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( classAHpp.checkIncludesCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t6_2_ignore_files_ignore_source )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject(
		"main.cpp",
		R"(
		#include <boost/test/unit_test.hpp>
		#include "lib/classA.hpp"
		)" );
	addFileToProject(
		"lib_impl/classA.cpp",
		R"(
		#include "lib/classA.hpp"
		)" );

	addFileFilter( "boost/.*" );
	addFileFilter( "lib_impl/.*" );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( "/test_project/main.cpp" );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClass = mainCpp.getInclude( 0 );
			BOOST_CHECK(
				includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			BOOST_CHECK(
				includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			BOOST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHpp = model.findFile( "lib/classA.hpp" );
		BOOST_REQUIRE( classAHpp.isAvailable() );
		{
			BOOST_REQUIRE( classAHpp.checkIncludedByCount( 1 ) );

			IncludeWrapper includeMainToClass =
				classAHpp.getIncludedBy( "/test_project/main.cpp" );
			BOOST_CHECK(
				includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			BOOST_CHECK(
				includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			BOOST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( classAHpp.checkIncludesCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t7_1_file_extensions_only_cpp_files )
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
		classACppName, "#include \"" + classAHppName +
						   "\"		\n"
						   "A::A();						\n" );

	addCppExtensions( { "*.cpp", "*.hpp", "*.hxx" } );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 4 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 3 ) );

			IncludeWrapper includedByMain =
				classAHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includedByClassA =
				classAHeader.getIncludedBy( classACppPath );
			BOOST_CHECK( includedByClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		BOOST_REQUIRE( classACpp.isAvailable() );
		{
			BOOST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			BOOST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			BOOST_CHECK(
				includeClassAToHeader.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper cppheaderFile = model.findFile( cppHeaderPath );
		BOOST_REQUIRE( cppheaderFile.isAvailable() );
		{
			BOOST_REQUIRE( cppheaderFile.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader =
				cppheaderFile.getInclude( 0 );
			BOOST_CHECK( includeClassAToHeader.checkSource( cppHeaderPath ) );
			BOOST_CHECK(
				includeClassAToHeader.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( cppheaderFile.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t7_2_file_extensions_analyze_without_extension )
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
		classACppName, "#include \"" + classAHppName +
						   "\"		\n"
						   "A::A();						\n" );

	addCppExtensions( { "*.cpp", "*.hpp" } );
	setAnalyzeWithoutExtension( true );

	// Run
	ModelWrapper model = analyze();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 4 ) );
	{
		FileWrapper mainCpp = model.findFile( mainCppPath );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( mainCppPath ) );
			BOOST_CHECK(
				includeMainToClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 3 ) );

			IncludeWrapper includedByMain =
				classAHeader.getIncludedBy( mainCppPath );
			BOOST_CHECK( includedByMain.checkSource( mainCppPath ) );
			BOOST_CHECK( includedByMain.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByMain.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByMain.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includedByClassA =
				classAHeader.getIncludedBy( classACppPath );
			BOOST_CHECK( includedByClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		BOOST_REQUIRE( classACpp.isAvailable() );
		{
			BOOST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			BOOST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			BOOST_CHECK(
				includeClassAToHeader.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper headerFile = model.findFile( headerPath );
		BOOST_REQUIRE( headerFile.isAvailable() );
		{
			BOOST_REQUIRE( headerFile.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = headerFile.getInclude( 0 );
			BOOST_CHECK( includeClassAToHeader.checkSource( headerPath ) );
			BOOST_CHECK(
				includeClassAToHeader.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			BOOST_CHECK(
				includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( headerFile.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
