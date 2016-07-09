--This file gets run when the game ends

player:unspawn()
print(#enemies)
for i,v in ipairs(enemies) do
	v:unspawn()
end
--Old C Code:
-- unspawnEntity(player);
-- deleteEntity(player);
