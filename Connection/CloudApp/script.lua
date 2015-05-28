function map(data)
   outTable = {}
   sum = 0
   for k, v in pairs(data) do
       if tonumber(k) > 2 then
           outTable[tostring(k)] = tostring(v)
           sum = sum + tonumber(v)
       end
   end

   if tonumber(data[1]) < 0 then
       outTable['-1'] = sum / 12
   else
       outTable['-2'] = sum / 12
   end
   lat_long = tostring(data[0]) ..'_' ..tostring(data[1])
   outTable[lat_long] = sum / 12
   return outTable;
end

function reduce(key, value)
   sum = 0
   i=0
   for k, v in pairs(value) do
       if( v ~= '') then 
           sum = sum + tonumber(v)
       end
   i=i+1
   end
   res = sum / i
   return key, tostring(res)
end

function main()
   funName = 'map'
   print('start map')
   res2 = doMap(funName, in1)
   print('start reduce')
   res3 = doReduce('reduce', res2)
   print('end reduce')
end;