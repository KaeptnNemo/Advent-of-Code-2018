[<EntryPoint>]
let main args =
    let lines = System.IO.File.ReadLines "input.txt"
    let flagsForOne s =
        let occ = s |> Seq.countBy id |> Seq.map (fun (e,c) -> c)
        (Seq.contains 2 occ, Seq.contains 3 occ)
    let flags = Seq.map flagsForOne lines
    let foldFunc (c2,c3) (f2,f3) = (c2 + (if f2 then 1 else 0), c3 + (if f3 then 1 else 0))
    let (twice,thrice) = Seq.fold foldFunc (0,0) flags
    printfn "%i" (twice * thrice)
    0