-- Read-only, bounded discovery. No setters, input or playback changes.
local rows, seen, pending, ready = {}, {}, false, false
local function call(o,m,...) if not o then return end local ok,v=pcall(o.call,o,m,...) if ok then return v end end
local function vec(v)
    if not v then return end
    local r={} for _,k in ipairs({'x','y','z','w','h'}) do pcall(function() if type(v[k])=='number' then r[k]=v[k] end end) end
    return r
end
re.on_frame(function()
    if not ready and sdk.find_type_definition('via.movie.Movie') then
        ready=true local metadata={}
        for _,name in ipairs({'via.GameObject','via.gui.PlayObject','via.gui.TransformObject','via.gui.Texture','via.gui.Material','via.gui.Text','via.gui.View','via.movie.Movie','app.MovieManagerApp'}) do
            local t=sdk.find_type_definition(name) local chain={}
            while t and #chain<12 do
                local methods={} for _,m in ipairs(t:get_methods()) do
                    local r={name=m:get_name(),parameters={}}
                    pcall(function() r.returns=m:get_return_type():get_full_name() for _,p in ipairs(m:get_param_types()) do table.insert(r.parameters,p:get_full_name()) end end)
                    table.insert(methods,r)
                end
                table.insert(chain,{type=t:get_full_name(),methods=methods}) t=t:get_parent_type()
            end
            metadata[name]=chain
        end
        json.dump_file('RE8Display-media-r2-methods.json',metadata)
    end
    if pending then pending=false json.dump_file('RE8Display-media-r2-layers.json',rows) end
end)
re.on_pre_gui_draw_element(function(element)
    if not ready or #rows>=128 then return true end
    pcall(function()
        local owner=element:get_type_definition():get_full_name()=='via.GameObject' and element or call(element,'get_GameObject')
        local name=call(owner,'get_Name') if not name or seen[name] then return end
        local gui=call(owner,'getComponent(System.Type)',sdk.typeof('via.gui.GUI'))
        local view=call(gui,'get_View') if not view then return end
        seen[name]=true
        local movie=call(owner,'getComponent(System.Type)',sdk.typeof('via.movie.Movie'))
        local r={name=name,screen=vec(call(view,'get_ScreenSize')),movie=movie~=nil,width=call(movie,'get_VideoWidth'),height=call(movie,'get_VideoHeight'),nodes={}}
        local visited={} local queue={{view,'',0}} local index=1
        while index<=#queue and #r.nodes<512 do
            local q=queue[index] index=index+1 local node=q[1]
            if node and not visited[tostring(node)] then
                visited[tostring(node)]=true local n=call(node,'get_Name') or '?'
                local path=q[2]..'/'..n
                table.insert(r.nodes,{path=path,depth=q[3],type=node:get_type_definition():get_full_name(),size=vec(call(node,'get_Size')),position=vec(call(node,'get_Position')),scale=vec(call(node,'get_Scale')),adjust=call(node,'get_ResolutionAdjust'),mode=call(node,'get_ResAdjustScale'),anchor=call(node,'get_ResAdjustAnchor'),condition=call(node,'get_ResAdjustCondition')})
                if q[3]<20 then local child=call(node,'get_Child') if child then table.insert(queue,{child,path,q[3]+1}) end end
                local next=call(node,'get_Next') if next and q[3]>0 then table.insert(queue,{next,q[2],q[3]}) end
            end
        end
        table.insert(rows,r) pending=true
        log.info('[RE8Display.MediaPromptInspect] layer='..name..' movie='..tostring(r.movie)..' nodes='..#r.nodes)
    end)
    return true
end)
