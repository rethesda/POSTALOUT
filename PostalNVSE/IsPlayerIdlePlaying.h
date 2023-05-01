#pragma once

#define CALL_EAX(addr) __asm mov eax, addr __asm call eax
__declspec(naked) TESIdleForm* AnimData::GetPlayedIdle()
{
	__asm
	{
		mov		eax, [ecx + 0x128]
		test	eax, eax
		jz		noQueued
		mov		eax, [eax + 0x2C]
		test	eax, eax
		jnz		done
		noQueued :
		mov		eax, [ecx + 0x124]
			test	eax, eax
			jz		done
			mov		eax, [eax + 0x2C]
			test	eax, eax
			jz		done
			push	eax
			CALL_EAX(0x4985F0)
			pop		edx
			movzx	eax, al
			dec		eax
			and eax, edx
			done :
		retn
	}
}


bool __fastcall IsPlayerIdlePlayingCall(TESIdleForm* idleAnim)
{
	PlayerCharacter* thePlayer = 0;
	thePlayer = PlayerCharacter::GetSingleton();

	AnimData* animData3rd = thePlayer->baseProcess->GetAnimData();
	AnimData* animData1st = thePlayer->firstPersonAnimData;

	return (animData3rd->GetPlayedIdle() == idleAnim) || (animData1st->GetPlayedIdle() == idleAnim);
}

#if RUNTIME
bool Cmd_IsPlayerIdlePlaying_Execute(COMMAND_ARGS)
{
	TESIdleForm* idleAnim;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &idleAnim) && IsPlayerIdlePlayingCall(idleAnim))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_IsPlayerIdlePlaying_Eval(COMMAND_ARGS_EVAL)
{
	*result = IsPlayerIdlePlayingCall((TESIdleForm*)arg1);
	return true;
}
#endif
DEFINE_CMD_COND_PLUGIN(IsPlayerIdlePlaying, 0, 1, kParams_OneIdleForm);