[<EntryPoint>]
let main args =
    let cmpIds s1 s2 =
        Seq.zip s1 s2 |> Seq.fold (fun s (x,y) -> (if x<>y then 1 else 0) + s) 0

    let buildCommon (s1,s2) =
        Seq.zip s1 s2 |> Seq.fold (fun s (x,y) -> s + (if x<>y then "" else (string x))) ""

    let lines = "input.txt" |> System.IO.File.ReadLines |> Seq.toArray
    let pairs = seq {
        for line in lines do
            for other in lines do
                if (cmpIds line other) = 1 then
                    yield (line, other)
    }
    pairs |> Seq.head |> buildCommon |> printfn "%s"
    0
