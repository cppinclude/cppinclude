git_clone()
{
	local gitUrl=$1
	local tagName=$2
	
	git clone -b $gitUrl --depth 1 $tagName
}

run_analyze()
{
	local outPutFile="output_unix.txt"
	cppinclude > $outPutFile
	cat $outPutFile
}

run_analyze_git_repo()
{
	local gitUrl=$1
	local tagName=$2
	
	git_clone $gitUrl $tagName
	run_analyze	
}


