import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.mapreduce.Reducer;

import java.io.IOException;


class SumReducer extends Reducer<MatrixCoords, DoubleWritable, MatrixCoords, DoubleWritable> {

    @Override
    protected void reduce(MatrixCoords key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {
        double sum = 0;
        for (DoubleWritable value : values) {
            sum += value.get();
        }
        context.write(key, new DoubleWritable(sum));
    }
}
