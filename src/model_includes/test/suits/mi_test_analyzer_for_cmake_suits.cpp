#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include "model_includes/test/fixtures/wrappers/mi_test_model_wrapper.hpp"
#include "model_includes/test/fixtures/wrappers/mi_test_file_wrapper.hpp"
#include "model_includes/test/fixtures/wrappers/mi_test_include_wrapper.hpp"

#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/system/config.hpp>

#include <std_fs>

/*------------------------------------------------------------------------------

TEST PLAN:

1. One cpp file in cmake
	1.1 Only one cpp file
	1.2 Cpp file with header
	1.3 Cpp file with header that include header
	1.4 cpp file with system include
2. Several cpp files in cmake
	2.1 Several files
	2.2 Several files with same header
3. Ignore files
	3.1 Ignore destination
	3.2 Ignore source

------------------------------------------------------------------------------*/

namespace model_includes::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(AnalyzerforCmakeTests, ModelIncludesFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_1_only_one_cpp_file)
{
	// Init
	const std::string projectDir{ "/test_project/" };
	const std::string classACppName{ "class_a.cpp" };
	const std::string classACppPath{ projectDir + classACppName };

	setProjectDir( projectDir );

	addFileToProject( classACppName, "void f(){}" );
	addFileToCmake( classACppPath );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 1 ) );
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		BOOST_REQUIRE( classACpp.isAvailable() );
		{
			BOOST_REQUIRE( classACpp.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_2_cpp_file_with_header)
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classAHppName{ "class_a.hpp" };
	const std::string classACppName{ "class_a.cpp" };

	const std::string classAHppPath{ projectDir + classAHppName };
	const std::string classACppPath{ projectDir + classACppName };

	setProjectDir( projectDir );

	addFileToProject( classAHppName, "class A{ A();  };" );
	addFileToProject(
		classACppName,
		"#include \"" + classAHppName + "\"	\n"
		"A::A();"
	);

	addFileToCmake( classACppPath );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByClassA = classAHeader.getIncludedBy( classACppPath );
			BOOST_CHECK( includedByClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			BOOST_CHECK( includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		BOOST_REQUIRE( classACpp.isAvailable() );
		{
			BOOST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			BOOST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			BOOST_CHECK( includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_4_cpp_file_with_system_include)
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string systemHeaderName{ "iostream" };
	const std::string classACppName{ "class_a.cpp" };

	const std::string classACppPath{ projectDir + classACppName };

	setProjectDir( projectDir );

	addFileToProject(
		classACppName,
		"#include <" + systemHeaderName + ">\n"
		"void f(){}"
	);

	addFileToCmake( classACppPath );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper systemHeader = model.findFile( systemHeaderName );
		BOOST_REQUIRE( systemHeader.isAvailable() );
		{
			BOOST_REQUIRE( systemHeader.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( systemHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByClassA = systemHeader.getIncludedBy( classACppPath );
			BOOST_CHECK( includedByClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includedByClassA.checkDestination( systemHeaderName ) );
			BOOST_CHECK( includedByClassA.checkType( IncludeType::System ) );
			BOOST_CHECK( includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		BOOST_REQUIRE( classACpp.isAvailable() );
		{
			BOOST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			BOOST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkDestination( systemHeaderName ) );
			BOOST_CHECK( includeClassAToHeader.checkType( IncludeType::System ) );
			BOOST_CHECK( includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_3_cpp_file_with_header_that_include_header)
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classBaseHppName{ "class_base.hpp" };
	const std::string classAHppName{ "class_a.hpp" };
	const std::string classACppName{ "class_a.cpp" };

	const std::string classBaseHppPath{		projectDir + classBaseHppName };
	const std::string classAHppPath{		projectDir + classAHppName };
	const std::string classACppPath{		projectDir + classACppName };

	setProjectDir( projectDir );

	addFileToProject( classBaseHppName, "class Base {};" );
	addFileToProject( classAHppName,
		"#include \"" + classBaseHppName + "\" \n"
		"class A : public Base { A();  };"
	);
	addFileToProject(
		classACppName,
		"#include \"" + classAHppName + "\"		\n"
		"A::A();"
	);

	addFileToCmake( classACppPath );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper classBaseHeader = model.findFile( classBaseHppPath );
		BOOST_REQUIRE( classBaseHeader.isAvailable() );
		{
			BOOST_REQUIRE( classBaseHeader.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( classBaseHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByClassA = classBaseHeader.getIncludedBy( classAHppPath );
			BOOST_CHECK( includedByClassA.checkSource( classAHppPath ) );
			BOOST_CHECK( includedByClassA.checkDestination( classBaseHppPath ) );
			BOOST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			BOOST_CHECK( includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classAHeader = model.findFile( classAHppPath );
		BOOST_REQUIRE( classAHeader.isAvailable() );
		{
			BOOST_REQUIRE( classAHeader.checkIncludesCount( 1 ) );

			IncludeWrapper includeToBaseClass = classAHeader.getInclude( 0 );
			BOOST_CHECK( includeToBaseClass.checkSource( classAHppPath ) );
			BOOST_CHECK( includeToBaseClass.checkDestination( classBaseHppPath ) );
			BOOST_CHECK( includeToBaseClass.checkType( IncludeType::User ) );
			BOOST_CHECK( includeToBaseClass.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classAHeader.checkIncludedByCount( 1 ) );

			IncludeWrapper includedByClassA = classAHeader.getIncludedBy( classACppPath );
			BOOST_CHECK( includedByClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includedByClassA.checkDestination( classAHppPath ) );
			BOOST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			BOOST_CHECK( includedByClassA.checkStatus( IncludeStatus::Resolved ) );
		}
	}
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		BOOST_REQUIRE( classACpp.isAvailable() );
		{
			BOOST_REQUIRE( classACpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeClassAToHeader = classACpp.getInclude( 0 );
			BOOST_CHECK( includeClassAToHeader.checkSource( classACppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkDestination( classAHppPath ) );
			BOOST_CHECK( includeClassAToHeader.checkType( IncludeType::User ) );
			BOOST_CHECK( includeClassAToHeader.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_1_several_files)
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classACppName{ "class_a.cpp" };
	const std::string classBCppName{ "class_b.cpp" };

	const std::string classACppPath{ projectDir + classACppName };
	const std::string classBCppPath{ projectDir + classBCppName };

	setProjectDir( projectDir );

	addFileToProject( classACppName, "void f(){}" );
	addFileToProject( classBCppName, "void f(){}" );

	addFileToCmake( classACppPath );
	addFileToCmake( classBCppPath );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper classACpp = model.findFile( classACppPath );
		BOOST_REQUIRE( classACpp.isAvailable() );
		{
			BOOST_REQUIRE( classACpp.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( classACpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classBCpp = model.findFile( classBCppPath );
		BOOST_REQUIRE( classBCpp.isAvailable() );
		{
			BOOST_REQUIRE( classBCpp.checkIncludesCount( 0 ) );
			BOOST_REQUIRE( classBCpp.checkIncludedByCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_2_several_files_with_same_header)
{
	// Init
	const std::string projectDir{ "/test_project/" };

	const std::string classACppName{ "class_a.cpp" };
	const std::string classBCppName{ "class_b.cpp" };
	const std::string classHeaderName{ "class_header.hpp" };

	const std::string classACppPath{ projectDir + classACppName };
	const std::string classBCppPath{ projectDir + classBCppName };
	const std::string classHeaderPath{ projectDir + classHeaderName };

	setProjectDir( projectDir );

	addFileToProject(
		classACppName,
		"#include \"" + classHeaderName + "\" \n"
		"void bar(){ f(); }"
	);
	addFileToProject(
		classBCppName,
		"#include \"" + classHeaderName + "\" \n"
		"void foo(){ f(); }"
	);
	addFileToProject( classHeaderName, "void f(){}" );

	addFileToCmake( classACppPath );
	addFileToCmake( classBCppPath );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 3 ) );
	{
		FileWrapper classAFile = model.findFile( classACppPath );
		BOOST_REQUIRE( classAFile.isAvailable() );
		{
			BOOST_REQUIRE( classAFile.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = classAFile.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includeMainToClassA.checkDestination( classHeaderPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classAFile.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classBFile = model.findFile( classACppPath );
		BOOST_REQUIRE( classBFile.isAvailable() );
		{
			BOOST_REQUIRE( classBFile.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClassA = classBFile.getInclude( 0 );
			BOOST_CHECK( includeMainToClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includeMainToClassA.checkDestination( classHeaderPath ) );
			BOOST_CHECK( includeMainToClassA.checkType( IncludeType::User ) );
			BOOST_CHECK( includeMainToClassA.checkStatus( IncludeStatus::Resolved ) );

			BOOST_REQUIRE( classBFile.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper headerFile = model.findFile( classHeaderPath );
		BOOST_REQUIRE( headerFile.isAvailable() );
		{
			BOOST_REQUIRE( headerFile.checkIncludesCount( 0 ) );

			BOOST_REQUIRE( headerFile.checkIncludedByCount( 2 ) );

			IncludeWrapper includedByClassA = headerFile.getIncludedBy( classACppPath );
			BOOST_CHECK( includedByClassA.checkSource( classACppPath ) );
			BOOST_CHECK( includedByClassA.checkDestination( classHeaderPath ) );
			BOOST_CHECK( includedByClassA.checkType( IncludeType::User ) );
			BOOST_CHECK( includedByClassA.checkStatus( IncludeStatus::Resolved ) );

			IncludeWrapper includedByClassB = headerFile.getIncludedBy( classBCppPath );
			BOOST_CHECK( includedByClassB.checkSource( classBCppPath ) );
			BOOST_CHECK( includedByClassB.checkDestination( classHeaderPath ) );
			BOOST_CHECK( includedByClassB.checkType( IncludeType::User ) );
			BOOST_CHECK( includedByClassB.checkStatus( IncludeStatus::Resolved ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_1_ignore_files_ignore_destination)
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

	addFileToCmake( "/test_project/main.cpp" );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( "/test_project/main.cpp" );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClass = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			BOOST_CHECK( includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			BOOST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			BOOST_CHECK( includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHpp = model.findFile( "lib/classA.hpp" );
		BOOST_REQUIRE( classAHpp.isAvailable() );
		{
			BOOST_REQUIRE( classAHpp.checkIncludedByCount( 1 ) );

			IncludeWrapper includeMainToClass = classAHpp.getIncludedBy( "/test_project/main.cpp" );
			BOOST_CHECK( includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			BOOST_CHECK( includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			BOOST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			BOOST_CHECK( includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( classAHpp.checkIncludesCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_2_ignore_files_ignore_source)
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

	addFileToCmake( "/test_project/main.cpp" );
	addFileToCmake( "/test_project/lib/classA.hpp" );

	// Run
	ModelWrapper model = analyzeCmake();

	// Check
	BOOST_REQUIRE( model.isAvailable() );
	BOOST_REQUIRE( model.checkFilesCount( 2 ) );
	{
		FileWrapper mainCpp = model.findFile( "/test_project/main.cpp" );
		BOOST_REQUIRE( mainCpp.isAvailable() );
		{
			BOOST_REQUIRE( mainCpp.checkIncludesCount( 1 ) );

			IncludeWrapper includeMainToClass = mainCpp.getInclude( 0 );
			BOOST_CHECK( includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			BOOST_CHECK( includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			BOOST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			BOOST_CHECK( includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( mainCpp.checkIncludedByCount( 0 ) );
		}
	}
	{
		FileWrapper classAHpp = model.findFile( "lib/classA.hpp" );
		BOOST_REQUIRE( classAHpp.isAvailable() );
		{
			BOOST_REQUIRE( classAHpp.checkIncludedByCount( 1 ) );

			IncludeWrapper includeMainToClass = classAHpp.getIncludedBy( "/test_project/main.cpp" );
			BOOST_CHECK( includeMainToClass.checkSource( "/test_project/main.cpp" ) );
			BOOST_CHECK( includeMainToClass.checkDestination( "lib/classA.hpp" ) );
			BOOST_CHECK( includeMainToClass.checkType( IncludeType::User ) );
			BOOST_CHECK( includeMainToClass.checkStatus( IncludeStatus::Unresolved ) );

			BOOST_REQUIRE( classAHpp.checkIncludesCount( 0 ) );
		}
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
