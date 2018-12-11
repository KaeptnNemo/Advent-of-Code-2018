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

    let input =
        args
        |> Array.head
        |> System.IO.File.OpenText
        |> asCharSeq
        |> Seq.filter System.Char.IsLetter
        |> Seq.toArray

    let lengthes = seq {
        for c in 'A' .. 'Z' do
            yield
                input
                |> Seq.ofArray
                |> Seq.filter (fun x -> (System.Char.ToUpper x) <> c)
                |> Seq.fold updateStack []
                |> List.length
                |> (fun l -> (c,l))
    }

    lengthes |> Seq.minBy snd |> snd |> printfn "%i" |> (fun x -> 0)