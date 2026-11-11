-- Bounded, read-only media/transition inspection. No game setters or input.
local ready, rows, seen, pending = false, {}, {}, false
local function call(o,m,...) if not o then return nil end local ok,v=pcall(o.call,o,m,...) if ok then return v end end
local function pair(v)
    if not v then return nil end
    local function field(k) local ok,x=pcall(function() return v[k] end) if ok and type(x)=='number' then return x end end
    return {x=field('x'),y=field('y'),w=field('w'),h=field('h'),width=field('width'),height=field('height')}
end
re.on_frame(function()
    if not ready and sdk.find_type_definition('app.MovieManagerApp') then
        ready=true
        local metadata={}
        for _,name in ipairs({'via.movie.Movie','via.gui.GUI','via.gui.View','via.gui.Texture','via.gui.PlayObject','via.gui.TransformObject','app.MovieManagerApp','app.FadeManager'}) do
            local t=sdk.find_type_definition(name) local methods={}
            if t then for _,m in ipairs(t:get_methods()) do
                local r={name=m:get_name(),parameters={}}
                pcall(function() r.returns=m:get_return_type():get_full_name() for _,p in ipairs(m:get_param_types()) do table.insert(r.parameters,p:get_full_name()) end end)
                table.insert(methods,r)
            end end
            metadata[name]=methods
        end
        json.dump_file('RE8Display-media-metadata.json',metadata)
    end
    if pending then pending=false json.dump_file('RE8Display-media-layers.json',rows) end
end)
re.on_pre_gui_draw_element(function(element)
    if not ready or #rows>=256 then return true end
    pcall(function()
        local t=element:get_type_definition():get_full_name()
        local owner=t=='via.GameObject' and element or call(element,'get_GameObject')
        if not owner then return end
        local name=call(owner,'get_Name') if not name or seen[name] then return end
        local gui=call(owner,'getComponent(System.Type)',sdk.typeof('via.gui.GUI'))
        local view=call(gui,'get_View') if not view then return end
        seen[name]=true
        local movie=call(owner,'getComponent(System.Type)',sdk.typeof('via.movie.Movie'))
        local r={name=name,type=t,screen=pair(call(view,'get_ScreenSize')),movie=movie~=nil,width=call(movie,'get_VideoWidth'),height=call(movie,'get_VideoHeight'),nodes={}}
        local node=call(view,'get_Child') local count=0
        while node and count<16 do
            count=count+1 table.insert(r.nodes,{type=node:get_type_definition():get_full_name(),name=call(node,'get_Name'),size=pair(call(node,'get_Size')),position=pair(call(node,'get_Position'))})
            node=call(node,'get_Next')
        end
        table.insert(rows,r) pending=true
        log.info('[RE8Display.MediaInspect] layer='..name..' movie='..tostring(r.movie))
    end)
    return true
end)
