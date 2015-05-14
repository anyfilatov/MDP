function map(data)
 outTable = {}
 sum = 0
 for k, v in pairs(data) do
 if tonumber(k) > 2 then
 table.insert(outTable, tonumber(v))
 sum = sum + tonumber(v)
 end
 end

 if data[1] < 0 then
 outTable["-1"] = sum/12
 else 
 outTable["-2"] = sum/12
 end
 lat_long = tostring(data[1]) .."_" ..tostring(data[2])
 outTable[lat_long] = sum/12

 return outTable;
end 

function reduce(key, value)
 sum = 0

 for k,v in pairs(value) do 
 sum = sum + tonumber(v)
 end
 return key, sum/table.getn(value)
end 

function main()
 funName = "map"
 print("start map")
 res2 = doMap(funName, in0)
 print("start reduce")
 res3 = doReduce("reduce", res2)
 print("start reduce")
end