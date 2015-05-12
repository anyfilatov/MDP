
function map(data)
  print("in lua map")
  t = {}
  t[0] = data
  t[1] = "1"
  t[2] = "aaaa"
  t[3] = "2"
  return t
end 

function reduce(key, value)
  print(key)
  for k,v in pairs(value) do print(k,v) end
  return key, "1"
end 

function main()
  a = "test"
  print(a) 
  funName = "map"
  res2 = doMap(funName, in0)
  print("start reduce")
  res3 = doReduce("reduce", res2)
end
