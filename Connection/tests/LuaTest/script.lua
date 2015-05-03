
function map1(word)
  
   return word, 1
end

function map2(word)
  
   return word, 1
end

function sum(t, i)
    print (i, " t[i]" , t[i])
 i = i or 1
 if t[i] ~= nil then
   return t[i] + sum(t, i + 1)
 end
end

function reduce1(t1, t2)
  print (t1, t2);
  return t1, sum(t2)
  -- return f, sum(s)
end


function map_reduce()
  print("in0", in0);
   result1 = doMap(map1, in0);
   print("in2", in0);
   result2 = doReduce(reduce1, result1);
   --result1 = doMap(map2, result2);
   --test(result2)
end

map_reduce()
print("Hello!");