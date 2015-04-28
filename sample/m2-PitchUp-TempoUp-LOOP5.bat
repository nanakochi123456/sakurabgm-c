set PROJ=m2-PitchUp-TempoUp-LOOP5
call sakurabgmenv.bat
%SAKURABGM% replace m2.mml %PROJ%.mml Key=4 Tempo=240 LOOP=5
%CSAKURA% "%PROJ%.mml"
"%PROJ%.mid"
