#!/usr/bin/osascript

-- Helper to map special key names to key codes
on ofSpecial(key)
	if key is "return" or key is "enter" then
		return 36
	else if key is "tab" then
		return 48
	else if key is "space" then
		return 49
	else if key is "delete" then
		return 51
	else if key is "escape" then
		return 53
	else if key is "left" then
		return 123
	else if key is "right" then
		return 124
	else if key is "down" then
		return 125
	else if key is "up" then
		return 126
	else
		return -1 -- invalid
	end if
end ofSpecial

-- Main logic
on run argv
	if (count of argv) = 0 then
		display dialog "Please provide title of app and a string to send."
		return
	end if

	set targetApp to item 1 of argv
	set keyToSend to item 2 of argv

	set specialKeys to {"return", "enter", "tab", "space", "delete", "escape", "up", "down", "left", "right"}

	-- Save the current frontmost app
	tell application "System Events"
		set frontAppName to name of first application process whose frontmost is true
	end tell

	-- Target app
	-- set targetApp to "TextEdit"

	-- Activate the target app
	tell application targetApp to activate
	delay 0.2

	-- Send key
	if specialKeys contains keyToSend then
		set keyCode to ofSpecial(keyToSend)
		if keyCode is not -1 then
			tell application "System Events" to key code keyCode
		end if
	else
		tell application "System Events" to keystroke keyToSend
	end if

	-- Return focus
	tell application frontAppName to activate
end run
