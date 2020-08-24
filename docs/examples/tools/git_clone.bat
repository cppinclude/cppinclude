set gitUrl=%1%
set tagName=%2%
git clone -b %gitUrl% --depth 1 %tagName%
