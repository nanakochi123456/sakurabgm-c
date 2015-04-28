/*
=title        "gs.h" ver.0.70
=description  �f�r�������L�̖��߂��`�����֐� (1999/12/17)
=keywords     SAKURA Function gs
*/

/** �T�v
��Roland GS �����ɓ��L�̖��߂��`�����֐��ł��B

** �g����
	Include(gs.h);
	�̈ꕶ���A�ȓ��ɏ����Ă��������B
*/

Include(stdmsg.h);
//------------------------------------------------------------------------------
// EFFECT

Function GSEffect(num,val){
	SysEx$=F0, 41, (DeviceNumber), 42, 12, {40, 01, (num), (val)}, F7;
}

/** ���o�[�u�Ɋւ��閽��
** GSReverbMacro(val)
�����o�[�u�i�c�����j�}�N���̕ύX�����܂��B
	val =
		00�FRoom 1	01�FRoom 2	02�FRoom 3
		03�FHall 1	04�FHall 2	05�FPlate
		06�FDelay	07�FPanning Delay
*/
Function GSReverbMacro(val)		{ GSEffect($30,val); }

/*** GSReverbCharacter(val)
�����o�[�u�i�c�����j�̌���ύX�����܂��B
	val =
		00�FRoom 1	01�FRoom 2	02�FRoom 3
		03�FHall 1	04�FHall 2	05�FPlate
		06�FDelay	07�FPanning Delay
*/
Function GSReverbCharacter(val)	{ GSEffect($31,val); }
/*
** GSReverbPRE_LPE(val)
** GSReverbLevel(val)
** GSReverbTime(val)
** GSReverbFeedback(val)
** GSReverbSendToChorus(val)
*/
Function GSReverbPRE_LPE(val)		{ GSEffect($32,val); }
Function GSReverbLevel(val)		{ GSEffect($33,val); }
Function GSReverbTime(val)		{ GSEffect($34,val); }
Function GSReverbFeedback(val)		{ GSEffect($35,val); }
Function GSReverbSendToChorus(val)	{ GSEffect($36,val); }

/** �R�[���X�Ɋւ��閽��
** GSChorusMacro(val)
���R�[���X�}�N���̕ύX�����܂��B
	val =
		00�FChorus 1      01�FChorus 2            02�FChorus 3
		03�FChorus 4      04�FFeedback Chorus     05�FFlanger
		06�FShort Delay   07�FShort Delay�iFB
*/
Function GSChorusMacro(val)		{ GSEffect($38,val); }
/*
**GSChorusPRE_LPF(val)
**GSChorusLevel(val)
**GSChorusFeedback(val)
**GSChorusDelay(val)
**GSChorusRate(val)
**GSChorusDepth(val)
**GSChorusSendToReverb(val)
**GSChorusSendToDelay(val)
*/
Function GSChorusPRE_LPF(val)		{ GSEffect($39,val); }
Function GSChorusLevel(val)		{ GSEffect($3A,val); }
Function GSChorusFeedback(val)		{ GSEffect($3B,val); }
Function GSChorusDelay(val)		{ GSEffect($3C,val); }
Function GSChorusRate(val)		{ GSEffect($3D,val); }
Function GSChorusDepth(val)		{ GSEffect($3E,val); }
Function GSChorusSendToReverb(val)	{ GSEffect($3F,val); }
Function GSChorusSendToDelay(val)	{ GSEffect($40,val); }

//------------------------------------------------------------------------------
//  ScaleTune

Function GSScaleTuning(C,Cp,D,Dp,E,F,Fp,G,Gp,A,Ap,B){
	FOR(Int I=$11; I <= $1F ; I++){
		SysEx = $F0,$41,(DeviceNumber),$42,$12,{$40,(I),$40,(C),(Cp),(D),(Dp),(E),(F),(Fp),(G),(Gp),(A),(Ap),(B)},$F7;
	}
}
