--This file gets run when the game ends

player:unspawn():delete()
for i,v in ipairs(enemies) do
	v:unspawn():delete()
end
--Old C Code:
-- unspawnEntity(player);
-- deleteEntity(player);