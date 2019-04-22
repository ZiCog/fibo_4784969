numeric digits 1000000
say fibo(4784969)
exit


fibo:
  parse arg n

  a = 0
  b = 1
  do i = 1 to length(strip(d2b(n), 'L','0'))
    d = a * (b * 2 - a)
    e = a * a + b * b
    a = d
    b = e
    if substr(strip(d2b(n), 'L','0'), i, 1) = '1' then
      do
        c = a + b
        a = b
        b = c
      end
  end
  return a


d2b: return x2b(d2x(arg(1)))

