
function map(data)
  print("in lua map")
  
  
  return outTable;
end 

function reduce(key, value)
  print(key)
  for k,v in pairs(value) do print(k,v) end
  return key, "srednee"
end 

function main()
  funName = "map"
  print("start map")
  res2 = doMap(funName, in0)
  print("start reduce")
  res3 = doReduce("reduce", res2)
  print("start reduce")
end