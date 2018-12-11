[<EntryPoint>]
let main args =

    let asCharSeq (stream: System.IO.StreamReader) = seq {
        let mutable last = stream.Read()
        while last <> -1 do
            yield (char last)
            last <- stream.Read()
    }

    let canReduce c1 c2 =
        c1 <> c2 && ((System.Char.ToUpper c1) = c2 || c1 = (System.Char.ToUpper c2))

    let updateStack s c =
        match s with
        | [] -> [c]
        | x::xs ->
            if canReduce x c then xs
            else c :: s

    args
    |> Array.head
    |> System.IO.File.OpenText
    |> asCharSeq
    |> Seq.filter System.Char.IsLetter
    |> Seq.fold updateStack []
    |> List.length
    |> printfn "%i"
    |> (fun x -> 0)
