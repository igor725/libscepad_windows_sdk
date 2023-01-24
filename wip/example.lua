local libScePad = require('libScePad')

local pad = assert(libScePad.open(1))
if pad then
	local state = libScePad.newPadState(1)

	while pad:read(state) do
		-- print(state.Buttons)
	end

	pad:close()
end
