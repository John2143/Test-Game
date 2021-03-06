local this = {}

function this:print(tab, spaces)
    spaces = spaces or 0
    for i,v in pairs(tab) do
        if type(v) == "table" then
            print((" "):rep(spaces) .. i .. ":")
            this:print(v, spaces + 2)
        else
            print((" "):rep(spaces) .. i .. ": " .. tostring(v))
        end
    end
end

--Stuff goes here if you need it to call some kind of extension to this, but
--  nothing will actually be initialized yet, though the lua engine is setup
--  including constructors (maps have not been memset, data has not been loaded
--  from lua)
function this:preInit()
    self.graphics = Graphics("Test", 800, 600);
    self.generateMap = require "lua/map"
    self.data = {
        entity = require "lua/entityData",
        items = require "lua/itemdata",
        bullets = require "lua/bulletData",
    }
end

--Everything is initialized, gameloop begins immediatly after this function ends
function this:postInit()
    self.player = Entity(0)
        :setControlled()
        :spawn()
        :setPos(700, 500)

    self.enemies = {}
    for i = 1, 10000 do
        self.enemies[i] = Entity(1)
            :grantAI(1) --AI_WANDER
            :setPos(300, 500)
            :spawn()
    end
    self.map = require "lua/map"
end

this.render = require "lua/render"

function this:render(t, dt)
    local g = self.graphics
    g:renderStart()
    g:renderTextJust("test", 50, 50, 4, 0);
    g:renderEnd()
end

--Gameloop just finished, save data and deconstruct things
function this:preExit()
    self.player:unspawn()
    print(#self.enemies)
    for i,v in ipairs(self.enemies) do
        v:unspawn()
    end
end

--Everything unloaded and deleted, probably nothing is left
function this:postExit()
end

return this
