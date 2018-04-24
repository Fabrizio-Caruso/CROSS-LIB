@echo off
if not "%1"=="" goto doit

echo If CPCfs creates a new image, it places the time and date of creation
echo after the required mark "MV - CPC". But CPE doesn't like it; it needs
echo the complete (original) tag. 
echo DROP_TAG.BAT does the work with CPCfs' COMMENT -D command.
echo.
echo Use: DROP_TAG (imagename)
goto end

:doit
echo Old and new comment of %1:
cpcfs %1 -e comment; comment -d; comment
:end
