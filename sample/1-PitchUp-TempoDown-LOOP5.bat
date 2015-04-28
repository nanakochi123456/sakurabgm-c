set PROJ=1-PitchUp-TempoDown-LOOP5
call sakurabgmenv.bat
%SAKURABGM% replace 1.mml %PROJ%.mml Key=2 Tempo=90 LOOP=5
%CSAKURA% "%PROJ%.mml"
"%PROJ%.mid"
