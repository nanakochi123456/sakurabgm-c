set PROJ=m2-PitchDown-TempoDown-LOOP5
call sakurabgmenv.bat
%SAKURABGM% replace m2.mml %PROJ%.mml Key=-2 Tempo=120 LOOP=5
%CSAKURA% "%PROJ%.mml"
"%PROJ%.mid"
