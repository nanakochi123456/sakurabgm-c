set PROJ=1-PitchDown-TempoUp-LOOP5
call sakurabgmenv.bat
%SAKURABGM% replace 1.mml %PROJ%.mml Key=-1 Tempo=140 LOOP=5
%CSAKURA% "%PROJ%.mml"
"%PROJ%.mid"
