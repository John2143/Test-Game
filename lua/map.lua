local size = 500
local map = Map(size)

--TODO maybe move this to C, call it reset
for y = 1, size do
    for x = 1, size do
        map:set(x - 1, y - 1, 3);
    end
end

--TODO map gen code

map:setCurrent();
return map
