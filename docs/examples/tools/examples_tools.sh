git_clone()
{
	local gitUrl=$1
	local tagName=$2
	
	echo $gitUrl
	echo $tagName
	
	git clone -b $tagName --depth 1 $gitUrl
}

#-------------------------------------------------------------------------------

run_analyze()
{
	local outPutFile="output_unix.txt"
	cppinclude > $outPutFile
	cat $outPutFile
}

#-------------------------------------------------------------------------------

run_analyze_git_repo()
{
	local gitUrl=$1
	local tagName=$2
	
	git_clone $gitUrl $tagName
	run_analyze	
}

#-------------------------------------------------------------------------------

run_cmake()
{
	local projectFolder=$1
	
	mkdir build
	cd build
	cmake ../$projectFolder -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cd -
}

#-------------------------------------------------------------------------------

run_analyze_git_repo_cmake()
{
	local gitUrl=$1
	local tagName=$2
	local projectFolder=$3
	
	run_cmake $projectFolder
	run_analyze_git_repo $gitUrl $tagName
}

#-------------------------------------------------------------------------------

clean_example_dir()
{
	local exampleDir=$1
	
	cd $exampleDir
	
	for dir in ./*/ ; 
		do (rm -rf $dir); 
	done
	
	cd -
}

#-------------------------------------------------------------------------------

run_example()
{
	local exampleDir=$1
	
	if [ "$exampleDir" != "./tools/" ]; then
	
		cd $exampleDir
		./run.sh
		cd -
		
		clean_example_dir $exampleDir
	fi
}

