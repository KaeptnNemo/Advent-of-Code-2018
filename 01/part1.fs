[<EntryPoint>]
let main args =
    args
    |> Array.head
    |> System.IO.File.ReadLines
    |> Seq.map int
    |> Seq.fold (fun acc x -> acc + x) 0
    |> printfn "%i"
    |> (fun x -> 0)