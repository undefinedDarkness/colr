local exe = arg[1]
if not exe then
	error("No screenshot argument given >:(")
end

local lgi = require('lgi')
Gtk = lgi.require("Gtk", "3.0")
Gdk = lgi.Gdk

local provider = Gtk.CssProvider()

-- Show a message if custom.css does not exist
assert(provider:load_from_path('app.css'), 'ERROR: file.css not found')

--- I add my CSS to the current window
local screen = Gdk.Display.get_default_screen(Gdk.Display:get_default())
local GTK_STYLE_PROVIDER_PRIORITY_APPLICATION = 600
Gtk.StyleContext.add_provider_for_screen(
	screen, provider,
	GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
)

function color_to_rgb(color)
	return  {
        r = tonumber("0x" .. color:sub(2, 3)),
        g = tonumber("0x" .. color:sub(4, 5)),
        b = tonumber("0x" .. color:sub(6, 7)),
    }
end

function color_to_hsv(r, g, b)
	local h, s, l = Gtk.rgb_to_hsv(r/255, g/255, b/255)
	h =  h * 255
	if h * 255 > 0 then
		h = h + 10 
		-- This is probably going to cause some issues!
		-- But it gives me results that are close enough for now so meh.
	end
	return string.format("hsv(%.0f, %d%%, %d%%)", h, math.ceil(s*100), math.ceil(l*100))
end


function color_lighten(color, amount)
    amount = amount or 26
    local c = color_to_rgb(color)

    c.r = c.r + amount
    c.r = c.r < 0 and 0 or c.r
    c.r = c.r > 255 and 255 or c.r
    c.g = c.g + amount
    c.g = c.g < 0 and 0 or c.g
    c.g = c.g > 255 and 255 or c.g
    c.b = c.b + amount
    c.b = c.b < 0 and 0 or c.b
    c.b = c.b > 255 and 255 or c.b

    return string.format("#%02x%02x%02x", c.r, c.g, c.b)
end

function pick()
	local cmd = io.popen(exe, 'r')
	local out = cmd:read('*a')
	cmd:close()
	return out:sub(1):gsub("\n", "")
end

function style(widget, css)
	local prov = Gtk.CssProvider()
	prov:load_from_data("* {"..css.."}")
	local contx = widget:get_style_context()
	contx:add_provider(prov, 600)
	return widget
end

function class(widget, class)
	local contx = widget:get_style_context()
	contx:add_class(class)
	return widget
end

-- TODO: Dont rengenerate the panel for every color..
function create_panel_for(hex) 
	local rgb = color_to_rgb(hex)
	local hsv = color_to_hsv(rgb.r, rgb.g, rgb.b)
	local amount = 38
	local view = Gtk.Box{
		class(Gtk.Box {
			homogeneous = true,
			-- style(Gtk.Box {}, "background-color:"..color_lighten(hex, 2*amount)),
			style(Gtk.Box {}, "background-color:"..color_lighten(hex, amount)),
			style(Gtk.Box {}, "background-color: "..hex),
			style(Gtk.Box {}, "background-color:"..color_lighten(hex, -amount)),
			-- style(Gtk.Box {}, "background-color:"..color_lighten(hex, -amount*2))
		}, "color-display"),
		class(Gtk.Box {
			class(Gtk.Label { label = "HEX" }, "color-label"),
			Gtk.Label { label = hex },
			spacing = 15
		}, "color-row"),
		class(Gtk.Box {
			class(Gtk.Label { label = "RGB" }, "color-label"),
			Gtk.Label { label = string.format("rgb(%d, %d, %d)", rgb.r, rgb.g, rgb.b) },
			spacing = 15
		}, "color-row"),
		class(Gtk.Box {
			class(Gtk.Label { label = "HSV" }, "color-label"),
			Gtk.Label { label = hsv },
			spacing = 15
		}, "color-row"),
		orientation = 1
	}
	view:show_all()
	return view
end

local layout = Gtk.HBox {}
local sidebar = Gtk.VBox {}
local preview
class(sidebar, "sidebar")
local picker_button = Gtk.Button.new_from_icon_name("color-picker", Gtk.IconSize.BUTTON)
function picker_button.on_clicked()
	local hex = pick()
	-- TODO: 
	-- Deal with duplicates

	local button = Gtk.Button{}
	style(button, "background-color: "..hex)
	class(button, "color-button")
	local pane = create_panel_for(hex) 
	function button.on_clicked() 
		if preview then layout:remove(preview) end
		preview = pane
		layout:add(pane)
	end
	button:show_all()
	sidebar:pack_start(button, false, false, 0)
end

sidebar:pack_start(picker_button, false, false, 0)

local x = Gtk.ScrolledWindow { sidebar }
x:set_policy(Gtk.PolicyType.NEVER, Gtk.PolicyType.AUTOMATIC)
layout:pack_start(x, false, false, 0)

local window = Gtk.Window {
	title = "Colr Pickr",
	default_width = 400,
	default_height=  300,
	resizable = true,
	layout
}
window:set_keep_above(true)
window:show_all()

Gtk.main()
