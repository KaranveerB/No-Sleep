@echo off
@"%__AppDir__%powercfg.exe" -SetACValueIndex Scheme_Current Sub_Buttons LidAction 1
@"%__AppDir__%powercfg.exe" -SetDCValueIndex Scheme_Current Sub_Buttons LidAction 1
@"%__AppDir__%powercfg.exe" -SetActive Scheme_Current
echo Sleep on lid close should be restored. If not, look into Windows Power Plan.
pause