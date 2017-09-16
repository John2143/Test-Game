--struct entityData{
    --textureID tid;
    --Entity::coreStats stats; //Default stats of a entity
    --const char *name;
    --int scale;
--};
--
--struct coreStats{
    --int def, agi, vit, abi;
--} stats;

--Short init
local function si(arr)
    return {
        agi = arr[1],
        def = arr[2],
        abi = arr[3],
        vit = arr[4],
    }
end

return {
    {
        tid = loadTexture("player.png"),
        stats = si{1, 2, 3, 4},
        name = "Player",
        scale = 4,
    }, {
        tid = loadTexture("brick.png"),
        stats = si{1, 2, 3, 4},
        name = "Enemy",
        scale = 4,
    },
}
